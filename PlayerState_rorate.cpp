//PlayerState_rotate.h
//20106_�c���@�@
//24_08_28
#include "PlayerState_rorate.h"
#include "Player.h"
#include "PlayerState_underwater.h"
#include "Manager.h"
#include "Scene.h"
#include "ParticleSpraywarter.h"
#include "ParticleFireup.h"
#include "Time.h"
#include "Input.h"
#include "Score.h"
#include "DataManager.h"

void PlayerStateRotate::Update(){
	//��]�O�̃N�H�[�^�j�I��
	XMVECTOR currentQuat = XMLoadFloat4(&GetPlayer()->GetQuaternion());
	//��ԏ��Z��
	XMVECTOR worldZ = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	//��]---------------------------------------------------------------------
	//�L�[�{�[�h�ł̑���
	if (Input::GetKeyPress('W') && !_isNegaRotateY) {
		GetPlayer()->GetRotation().x += 0.2f;	//��]���J�E���g�p�ɉ��Z
		_angulvel.x = 0.2f;
		_isPosiRotateY = true;					//�t��]���ł��Ȃ��悤��
	}
	else if (Input::GetKeyPress('S') && !_isPosiRotateY) {
		GetPlayer()->GetRotation().x += -0.2f;
		_angulvel.x = -0.2f;
		_isNegaRotateY = true;

	}
	if (Input::GetKeyPress('A') && !_isPosiRotateX) {
		GetPlayer()->GetRotation().y += -0.2f;
		_angulvel.y = -0.2f;
		_isNegaRotateX = true;
	}
	else if (Input::GetKeyPress('D') && !_isNegaRotateX) {
		GetPlayer()->GetRotation().y += 0.2f;
		_angulvel.y = 0.2f;
		_isPosiRotateX = true;
	}

	//�R���g���[���ł̑���
	if (Input::GetLeftStickX(0) > STICK_DEADZONE) {
		GetPlayer()->GetRotation().y += Input::GetLeftStickY(0);
	}
	else if (Input::GetLeftStickX(0) < -STICK_DEADZONE) {
		GetPlayer()->GetRotation().y += Input::GetLeftStickY(0);
	}
	if (Input::GetLeftStickY(0) > STICK_DEADZONE) {
		GetPlayer()->GetRotation().x += Input::GetLeftStickX(0);
	}
	else if (Input::GetLeftStickY(0) < -STICK_DEADZONE) {
		GetPlayer()->GetRotation().x += Input::GetLeftStickX(0);
	}

	//����
	_angulvel *= _friction;

	//�c��]
	if (std::abs(_angulvel.x) > 0.001f) {
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(worldZ, _angulvel.x);

		// ���݂̉�]�N�H�[�^�j�I���ɐV������]���|�����킹��
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//�v�Z��̃N�H�[�^�j�I��������
		XMStoreFloat4(&GetPlayer()->GetQuaternion(), currentQuat);
		//�v���C���[��Up���X�V
		GetPlayer()->GetComponent<Transform>()->Update();
	}
	//�P���]
	if (std::abs(_angulvel.y) > 0.001f) {
		//�c��]��̃N�H�[�^�j�I�����Čv�Z
		currentQuat = XMLoadFloat4(&GetPlayer()->GetQuaternion());
		
		//���[���hY���i������j����ɂ�����]�N�H�[�^�j�I���𐶐�
		XMVECTOR yAxis = XMLoadFloat3(&GetPlayer()->GetUp());
		//�N�H�[�^�j�I����]
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, _angulvel.y);

		// ���݂̉�]�N�H�[�^�j�I���ɐV������]���|�����킹��
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//�v�Z��̃N�H�[�^�j�I��������
		XMStoreFloat4(&GetPlayer()->GetQuaternion(), currentQuat);
		//�v���C���[��Up���X�V
		GetPlayer()->GetComponent<Transform>()->Update();
	}


	// 1��]��������J�E���g����
	if (GetPlayer()->GetRotation().y >= XM_2PI) {
		_twistcount++;
		GetPlayer()->GetRotation().y -= XM_2PI;
	}
	else if (GetPlayer()->GetRotation().y <= -XM_2PI) {
		_twistcount++;
		GetPlayer()->GetRotation().y += XM_2PI;
	}

	if (GetPlayer()->GetRotation().x >= XM_2PI) {
		_rotcount++;
		GetPlayer()->GetRotation().x -= XM_2PI;
	}
	else if (GetPlayer()->GetRotation().x <= -XM_2PI) {
		_rotcount++;
		GetPlayer()->GetRotation().x += XM_2PI;
	}

	//��������
	GetPlayer()->Fall(Time::GetDeltaTime());
	
	//����������
	if (GetPlayer()->GetPosition().y < 0.0f) {
		//���ʂƂ̓���
		_dot = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&GetPlayer()->GetUp()), { 0.0f, 1.0f, 0.0f }));
		//��Βl�ɂ���
		_dot = fabs(_dot);

		//�t�@�C���ɏ�������
		DataManager::SaveData(_twistcount, _rotcount, _dot);

		Scene* scene;
		scene = Manager::GetScene();

		//���ς���G�t�F�N�g�̑傫��������
		float spray = 2.0f - _dot;
		//�G�t�F�N�g���o��
		scene->AddGameobject<SprayWater>(Effect, GetPlayer()->GetPosition(), 0.7f, spray);

		if (_dot == 1.0f) {
			GetPlayer()->PlayAudioSml();
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(20.0f, 0.0f, 40.0f), 2.0f, 0.3f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 50.0f);
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(80.0f, 0.0f, 0.0f), 1.5f, 0.4f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 40.0f);
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(20.0f, 0.0f, -40.0f), 1.0f, 0.35f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 30.0f);
		}
		else if (_dot == 0.0f) {
			GetPlayer()->PlayAudioBig();
		}
		else {
			GetPlayer()->PlayAudioMid();
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(20.0f, 0.0f, 40.0f), 2.0f, 0.3f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 50.0f);
		}

		//�d�͂�������
		GetPlayer()->GetAccel().y *= 0.5f;

		GetPlayer()->ChangeState(new PlayerStateUnderwater(GetPlayer()));
	}
}

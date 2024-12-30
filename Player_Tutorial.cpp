//Player_Tutorial.cpp
//20106_�c���@�@
//24_09_17
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "S_Game.h"
#include "Input.h"
#include "Player_Tutorial.h"
#include "C_AnimationModel.h"
#include "C_Transform.h"


Player_Tutorial::Player_Tutorial(const XMFLOAT3& pos, const XMFLOAT3& scl):
Player(pos, scl){
	SetPosition(pos);
	SetScale(scl);
}

void Player_Tutorial::Init(){
	//Player�������Ă��邱�Ƃ�������
	AddComponent<AnimationModel>(this);

	//���f���ǂݍ���
	GetComponent<AnimationModel>()->Load("asset\\model\\bikeman.fbx");

	//�V�F�[�_�[�Z�b�g
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\NormalLightingVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\NormalLightingPS.cso");

	SetRotation({ 0.0f, 0.0f, 0.0f });

	//Y�������90�x��]������X��������������
	XMVECTOR yAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, XM_PIDIV2);
	XMStoreFloat4(&GetQuaternion(), rotationQuat);
}

void Player_Tutorial::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

void Player_Tutorial::Update() {
	SetQuaternion(GetQuaternion());
	for (auto c : _components) {
		c->Update();
	}
	//��]�O�̃N�H�[�^�j�I��
	XMVECTOR currentQuat = XMLoadFloat4(&GetQuaternion());
	//��ԏ��Z��
	XMVECTOR worldZ = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	//��]---------------------------------------------------------------------
	//�L�[�{�[�h�ł̑���
	if (Input::GetKeyPress('W')) {
		_angulvel.x = 0.2f;
	}
	else if (Input::GetKeyPress('S')) {
		_angulvel.x = -0.2f;

	}
	if (Input::GetKeyPress('A')) {
		_angulvel.y = -0.2f;
	}
	else if (Input::GetKeyPress('D')) {
		_angulvel.y = 0.2f;
	}

	//����
	_angulvel *= _friction;

	//�c��]
	if (std::abs(_angulvel.x) > 0.001f &&
		!XMVector3Equal((worldZ), XMVectorZero())) {
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(worldZ, _angulvel.x);

		// ���݂̉�]�N�H�[�^�j�I���ɐV������]���|�����킹��
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//�v�Z��̃N�H�[�^�j�I��������
		XMStoreFloat4(&GetQuaternion(), currentQuat);
		//�v���C���[��Up���X�V
		GetComponent<Transform>()->Update();
	}
	//�P���]
	if (std::abs(_angulvel.y) > 0.001f &&
		!XMVector3Equal((worldZ), XMVectorZero())) {
		//�c��]��̃N�H�[�^�j�I�����Čv�Z
		currentQuat = XMLoadFloat4(&GetQuaternion());

		//���[���hY���i������j����ɂ�����]�N�H�[�^�j�I���𐶐�
		XMVECTOR yAxis = XMLoadFloat3(&GetUp());
		//�N�H�[�^�j�I����]
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, _angulvel.y);

		// ���݂̉�]�N�H�[�^�j�I���ɐV������]���|�����킹��
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//�v�Z��̃N�H�[�^�j�I��������
		XMStoreFloat4(&GetQuaternion(), currentQuat);
		//�v���C���[��Up���X�V
		GetComponent<Transform>()->Update();
	}
}

void Player_Tutorial::Draw(){
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//�N�H�[�^�j�I���ŉ�]
	rot = XMMatrixRotationQuaternion(XMLoadFloat4(&GetQuaternion()));
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;

	if (Manager::GetisDrawFromLight()) {
		Renderer::SetWorldMatrix(world);
	}
	else {
		Renderer::SetWorldMatrix(world, _prevworld);
	}

	for (auto c : _components) {
		c->Draw();
	}
}

void Player_Tutorial::Hit(GameObject* obj){
}

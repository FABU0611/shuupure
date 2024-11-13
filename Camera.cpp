//camera.cpp
//20106_�c���@�@
//24_05_08
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "Scene.h"
#include "Time.h"

void Camera::Init(){
	SetPosition({ 0.0f, 25.0f, 30.0f });
	_target = { 0.0f, 0.0f, 0.0f };

	GetRotation().x = XM_PIDIV4;

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = GetPosition().x - _target.x;
	vz = GetPosition().z - _target.z;
	_length = sqrtf(vx * vx + vz * vz);

	_cameraspeed = 0.07f;
}

void Camera::Uninit(){
}

void Camera::Update() {
	//���̃^�[�Q�b�g������Đ��`��ԂŊ��炩�ɃJ�����ړ����ł���

	float dt = Time::GetDeltaTime();

	Scene* scene;
	scene = Manager::GetScene();

	Player* player;
	player = scene->GetGameobject<Player>();


	// ���݂̃^�[�Q�b�g�ʒu���擾
	XMVECTOR currentTargetPosition = XMLoadFloat3(&_target);

	// �v���C���[�̌��݂̈ʒu���^�[�Q�b�g�ɐݒ�
	XMVECTOR playerPosition = XMLoadFloat3(&player->GetPosition());
	//�R�s�[�����
	XMVECTOR desiredTargetPosition = playerPosition;

	// ���`��Ԃ��g���ă^�[�Q�b�g�ʒu���X�V
	XMVECTOR interpolatedTargetPosition = XMVectorLerp(currentTargetPosition, desiredTargetPosition, 15.0f * dt);

	// �^�[�Q�b�g�ʒu��ݒ�
	XMStoreFloat3(&_target, interpolatedTargetPosition);


	//�J������]
	//��
	if (Input::GetKeyPress('I')) {
		GetRotation().x -= (XM_PIDIV4 * 0.05f);
	}
	//��
	if (Input::GetKeyPress('K')) {
		GetRotation().x += (XM_PIDIV4 * 0.05f);
	}
	if (Input::GetRightStickX(0) < -STICK_DEADZONE ||
		Input::GetRightStickX(0) > STICK_DEADZONE) {
		GetRotation().y += Input::GetRightStickX(0) * _cameraspeed;
	}
	//��
	if (Input::GetKeyPress('L')) {
		GetRotation().y += (XM_PIDIV2 * 0.05f);
	}
	//�E
	if (Input::GetKeyPress('J')) {
		GetRotation().y -= (XM_PIDIV2 * 0.05f);
	}
	if (Input::GetRightStickY(0) < -STICK_DEADZONE ||
		Input::GetRightStickY(0) > STICK_DEADZONE) {
		GetRotation().x -= Input::GetRightStickY(0) * _cameraspeed;
	}


	if (GetRotation().x < -XM_PIDIV2){
		GetRotation().x = -XM_PIDIV2;
	}
	if (GetRotation().x > XM_PIDIV2){
		GetRotation().x = XM_PIDIV2;
	}
	if (GetRotation().y > XM_2PI){
		GetRotation().y -= XM_2PI;
	}
	if (GetRotation().y < -XM_2PI){
		GetRotation().y += XM_2PI;
	}


	// ��]�s��̌v�Z
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, 0.0f);

	// ��{�̃J�����ʒu
	XMVECTOR defaultPosition = XMVectorSet(0.0f, 0.0f, -_length, 0.0f);

	// �V�����J�����ʒu���v�Z
	XMVECTOR rotatedPosition = XMVector3TransformCoord(defaultPosition, rotationMatrix);
	rotatedPosition += XMLoadFloat3(&_target);

	// �v�Z���ʂ� _transform.position �ɐݒ�
	XMStoreFloat3(&GetPosition(), rotatedPosition);

	//���ʂ�艺�ɍs���Ȃ��悤��
	if (GetPosition().y < 10.0f) {
		GetPosition().y = 10.0f;
	}
	if (_target.y < 0.0f) {
		_target.y = 0.0f;
	}
}

void Camera::Draw(){
	//�r���[�}�g���N�X�ݒ�
	XMFLOAT3 up{ 0.0f, 1.0f, 0.0f };
	XMMATRIX viewmatrix = XMMatrixLookAtLH(XMLoadFloat3(&GetPosition()), XMLoadFloat3(&_target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewmatrix, _prevview);

	XMStoreFloat4x4(&_viewmatrix, viewmatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix, _prevprojection);

	Renderer::SetCameraPosition(GetPosition());
}

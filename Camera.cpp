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
#include "S_Title.h"
#include "Time.h"
#include "CameraState_Title1.h"

void Camera::Init() {
	SetPosition({ 0.0f, 25.0f, 30.0f });
	_target = { 35.0f, 6.3f, 15.0f };

	GetRotation() = { -0.76f, -0.8f, 0.6f };

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = GetPosition().x - _target.x;
	vz = GetPosition().z - _target.z;
	_length = sqrtf(vx * vx + vz * vz);

	_cameraspeed = 0.07f;

	Scene* scene;
	scene = Manager::GetScene();

	Title* title = dynamic_cast<Title*>(scene);
	if (title) {
		_state = new TitleState1(this);
	}
}

void Camera::Uninit() {
	delete _state;
}

void Camera::Update() {
	_state->Update();

	XMFLOAT3 rot = GetRotation();

	// ��]�s��̌v�Z
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);

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

void Camera::Draw() {
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

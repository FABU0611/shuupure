//CameraState_Game1.cpp
//20106_�c���@�@
//24_12_12
#include "CameraState_Game1.h"
#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "Manager.h"
#include "Scene.h"
#include "Player.h"

const float GameState1::CAMERA_SPEED = 0.7f;

void GameState1::Update() {
	float dt = Time::GetDeltaTime();

	Camera* camera = GetCamera();

	Scene* scene;
	scene = Manager::GetScene();

	Player* player;
	player = scene->GetGameobject<Player>();


	if (player) {
		//���݂̃^�[�Q�b�g�ʒu���擾
		XMVECTOR currentTargetPosition = XMLoadFloat3(&camera->GetTartgetPos());

		//�v���C���[�̌��݂̈ʒu���^�[�Q�b�g�ɐݒ�
		XMVECTOR playerPosition = XMLoadFloat3(&player->GetPosition());
		//�R�s�[�����
		XMVECTOR desiredTargetPosition = playerPosition;
		//���`��Ԃ��g���ă^�[�Q�b�g�ʒu���X�V
		XMVECTOR interpolatedTargetPosition = XMVectorLerp(currentTargetPosition, desiredTargetPosition, 15.0f * dt);

		//�^�[�Q�b�g�ʒu��ݒ�
		XMStoreFloat3(&camera->GetTartgetPos(), interpolatedTargetPosition);

		//�J������]
		//��
		if (Input::GetKeyPress('I')) {
			camera->GetRotation().x -= (XM_PIDIV4 * 0.05f);
		}
		//��
		else if (Input::GetKeyPress('K')) {
			camera->GetRotation().x += (XM_PIDIV4 * 0.05f);
		}
		if (Input::GetRightStickX(0) < -STICK_DEADZONE ||
			Input::GetRightStickX(0) > STICK_DEADZONE) {
			camera->GetRotation().y += Input::GetRightStickX(0) * CAMERA_SPEED;
		}
		//��
		if (Input::GetKeyPress('L')) {
			camera->GetRotation().y += (XM_PIDIV2 * 0.05f);
		}
		//�E
		else if (Input::GetKeyPress('J')) {
			camera->GetRotation().y -= (XM_PIDIV2 * 0.05f);
		}
		if (Input::GetRightStickY(0) < -STICK_DEADZONE ||
			Input::GetRightStickY(0) > STICK_DEADZONE) {
			camera->GetRotation().x -= Input::GetRightStickY(0) * CAMERA_SPEED;
		}


		if (camera->GetRotation().x < -XM_PIDIV2) {
			camera->GetRotation().x = -XM_PIDIV2;
		}
		if (camera->GetRotation().x > XM_PIDIV2) {
			camera->GetRotation().x = XM_PIDIV2;
		}
		if (camera->GetRotation().y > XM_2PI) {
			camera->GetRotation().y -= XM_2PI;
		}
		if (camera->GetRotation().y < -XM_2PI) {
			camera->GetRotation().y += XM_2PI;
		}
		player->GetRotation().y = _camera->GetRotation().y;
	}
}

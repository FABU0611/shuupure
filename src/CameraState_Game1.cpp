//CameraState_Game1.cpp
//20106_田中　蓮
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
		//現在のターゲット位置を取得
		XMVECTOR currentTargetPosition = XMLoadFloat3(&camera->GetTartgetPos());

		//プレイヤーの現在の位置をターゲットに設定
		XMVECTOR playerPosition = XMLoadFloat3(&player->GetPosition());
		//コピーを作る
		XMVECTOR desiredTargetPosition = playerPosition;
		//線形補間を使ってターゲット位置を更新
		XMVECTOR interpolatedTargetPosition = XMVectorLerp(currentTargetPosition, desiredTargetPosition, 15.0f * dt);

		//ターゲット位置を設定
		XMStoreFloat3(&camera->GetTartgetPos(), interpolatedTargetPosition);

		//カメラ回転
		//上
		if (Input::GetKeyPress('I')) {
			camera->GetRotation().x -= (XM_PIDIV4 * 0.05f);
		}
		//下
		else if (Input::GetKeyPress('K')) {
			camera->GetRotation().x += (XM_PIDIV4 * 0.05f);
		}
		if (Input::GetRightStickX(0) < -STICK_DEADZONE ||
			Input::GetRightStickX(0) > STICK_DEADZONE) {
			camera->GetRotation().y += Input::GetRightStickX(0) * CAMERA_SPEED;
		}
		//左
		if (Input::GetKeyPress('L')) {
			camera->GetRotation().y += (XM_PIDIV2 * 0.05f);
		}
		//右
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

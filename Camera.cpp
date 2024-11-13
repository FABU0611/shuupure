//camera.cpp
//20106_田中　蓮
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

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = GetPosition().x - _target.x;
	vz = GetPosition().z - _target.z;
	_length = sqrtf(vx * vx + vz * vz);

	_cameraspeed = 0.07f;
}

void Camera::Uninit(){
}

void Camera::Update() {
	//仮のターゲットを作って線形補間で滑らかにカメラ移動ができる

	float dt = Time::GetDeltaTime();

	Scene* scene;
	scene = Manager::GetScene();

	Player* player;
	player = scene->GetGameobject<Player>();


	// 現在のターゲット位置を取得
	XMVECTOR currentTargetPosition = XMLoadFloat3(&_target);

	// プレイヤーの現在の位置をターゲットに設定
	XMVECTOR playerPosition = XMLoadFloat3(&player->GetPosition());
	//コピーを作る
	XMVECTOR desiredTargetPosition = playerPosition;

	// 線形補間を使ってターゲット位置を更新
	XMVECTOR interpolatedTargetPosition = XMVectorLerp(currentTargetPosition, desiredTargetPosition, 15.0f * dt);

	// ターゲット位置を設定
	XMStoreFloat3(&_target, interpolatedTargetPosition);


	//カメラ回転
	//上
	if (Input::GetKeyPress('I')) {
		GetRotation().x -= (XM_PIDIV4 * 0.05f);
	}
	//下
	if (Input::GetKeyPress('K')) {
		GetRotation().x += (XM_PIDIV4 * 0.05f);
	}
	if (Input::GetRightStickX(0) < -STICK_DEADZONE ||
		Input::GetRightStickX(0) > STICK_DEADZONE) {
		GetRotation().y += Input::GetRightStickX(0) * _cameraspeed;
	}
	//左
	if (Input::GetKeyPress('L')) {
		GetRotation().y += (XM_PIDIV2 * 0.05f);
	}
	//右
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


	// 回転行列の計算
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, 0.0f);

	// 基本のカメラ位置
	XMVECTOR defaultPosition = XMVectorSet(0.0f, 0.0f, -_length, 0.0f);

	// 新しいカメラ位置を計算
	XMVECTOR rotatedPosition = XMVector3TransformCoord(defaultPosition, rotationMatrix);
	rotatedPosition += XMLoadFloat3(&_target);

	// 計算結果を _transform.position に設定
	XMStoreFloat3(&GetPosition(), rotatedPosition);

	//水面より下に行かないように
	if (GetPosition().y < 10.0f) {
		GetPosition().y = 10.0f;
	}
	if (_target.y < 0.0f) {
		_target.y = 0.0f;
	}
}

void Camera::Draw(){
	//ビューマトリクス設定
	XMFLOAT3 up{ 0.0f, 1.0f, 0.0f };
	XMMATRIX viewmatrix = XMMatrixLookAtLH(XMLoadFloat3(&GetPosition()), XMLoadFloat3(&_target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewmatrix, _prevview);

	XMStoreFloat4x4(&_viewmatrix, viewmatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix, _prevprojection);

	Renderer::SetCameraPosition(GetPosition());
}

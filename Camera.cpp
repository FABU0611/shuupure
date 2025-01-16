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
#include "S_Title.h"
#include "Time.h"
#include "CameraState_Title1.h"
#include "CameraState_Game1.h"

const float Camera::NEAR_CLIP = 1.0f;
const float Camera::FAR_CLIP = 250.0f;

//カメラ視錐台を分割
void Camera::CalculationCascade() {
	float range = FAR_CLIP - NEAR_CLIP;
	float ratio = FAR_CLIP / NEAR_CLIP;

	for (int i = 0; i < CASCADE_NUM; i++) {
		float p = (i + 1) / (float)CASCADE_NUM;					//i = 0 -> 1/3, i = 1 -> 2/3...
		float logsplit = NEAR_CLIP * powf(ratio, p);			//対数分割　近いほど小さく分割
		float unifromsplit = NEAR_CLIP + range * p;				//線形分割　均等に分割
		_cascade[i] = std::lerp(logsplit, unifromsplit, 0.5f);	//ブレンドしていい感じに	遠くが広すぎると解像度が低くなる
	}

	Renderer::SetCascadeSplit(_cascade);
}

void Camera::Init() {
	CalculationCascade();

	SetPosition({ 0.0f, 25.0f, 30.0f });
	_target = { 35.0f, 6.3f, 15.0f };

	GetRotation() = { -0.76f, -0.8f, 0.6f };

	//視点と注視点の距離を計算
	float vx, vz;
	vx = GetPosition().x - _target.x;
	vz = GetPosition().z - _target.z;
	_length = sqrtf(vx * vx + vz * vz);

	_cameraspeed = 0.07f;

	_fov = 1.0f;
	_aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

	Scene* scene;
	scene = Manager::GetScene();

	Title* title = dynamic_cast<Title*>(scene);
	if (title) {
		_state = new TitleState1(this);
		return;
	}
	else {
		_state = new GameState1(this);
	}
}

void Camera::Uninit() {
	delete _state;
}

void Camera::Update() {
	if (_state) {
		_state->Update();
	}

	XMFLOAT3 rot = GetRotation();

	//回転行列の計算
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);

	//基本のカメラ位置
	XMVECTOR defaultPosition = XMVectorSet(0.0f, 0.0f, -_length, 0.0f);

	//新しいカメラ位置を計算
	XMVECTOR rotatedPosition = XMVector3TransformCoord(defaultPosition, rotationMatrix);
	rotatedPosition += XMLoadFloat3(&_target);

	//計算結果を _transform.position に設定
	XMStoreFloat3(&GetPosition(), rotatedPosition);

	//水面より下に行かないように
	if (GetPosition().y < 10.0f) {
		GetPosition().y = 10.0f;
	}
	if (_target.y < 0.0f) {
		_target.y = 0.0f;
	}

	GetComponent<Transform>()->Update();
}

void Camera::Draw() {
	//ビューマトリクス設定
	XMFLOAT3 up{ 0.0f, 1.0f, 0.0f };
	XMMATRIX viewmatrix = XMMatrixLookAtLH(XMLoadFloat3(&GetPosition()), XMLoadFloat3(&_target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewmatrix, _prevview);

	XMStoreFloat4x4(&_viewmatrix, viewmatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(_fov, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP, FAR_CLIP);

	Renderer::SetProjectionMatrix(projectionMatrix, _prevprojection);

	Renderer::SetCameraPosition(GetPosition());
}

//視錐台の８頂点をワールド座標にして出力
std::vector<XMVECTOR> Camera::GetCornersWorldSpace(const float& nearZ, const float& farZ) const {
	std::vector<XMVECTOR> corners(8);

	//近面の高さと幅の半分
	float nearHeight = tanf(_fov * 0.5f) * nearZ;
	float nearWidth = nearHeight * _aspect;
	//遠面の高さと幅の半分
	float farHeight = tanf(_fov * 0.5f) * farZ;
	float farWidth = farHeight * _aspect;

	//視錐台の四隅の座標を計算
	corners[0] = XMVectorSet(-nearWidth,  nearHeight, nearZ, 1.0f);
	corners[1] = XMVectorSet( nearWidth,  nearHeight, nearZ, 1.0f);
	corners[2] = XMVectorSet( nearWidth, -nearHeight, nearZ, 1.0f);
	corners[3] = XMVectorSet(-nearWidth, -nearHeight, nearZ, 1.0f);

	corners[4] = XMVectorSet(-farWidth,  farHeight, farZ, 1.0f);
	corners[5] = XMVectorSet( farWidth,  farHeight, farZ, 1.0f);
	corners[6] = XMVectorSet( farWidth, -farHeight, farZ, 1.0f);
	corners[7] = XMVectorSet(-farWidth, -farHeight, farZ, 1.0f);

	//逆変換してワールド空間に直す
	XMMATRIX invView = XMMatrixInverse(nullptr, XMLoadFloat4x4(&_viewmatrix));
	for (int i = 0; i < 8; i++) {
		corners[i] = XMVector3TransformCoord(corners[i], invView);
	}

	return corners;
}

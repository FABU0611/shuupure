//Player_Tutorial.cpp
//20106_田中　蓮
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
	//Playerが持っていることを教える
	AddComponent<AnimationModel>(this);

	//モデル読み込み
	GetComponent<AnimationModel>()->Load("asset\\model\\bikeman.fbx");

	//シェーダーセット
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\NormalLightingVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\NormalLightingPS.cso");

	SetRotation({ 0.0f, 0.0f, 0.0f });

	//Y軸周りに90度回転させてX方向を向かせる
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
	//回転前のクォータニオン
	XMVECTOR currentQuat = XMLoadFloat4(&GetQuaternion());
	//空間上のZ軸
	XMVECTOR worldZ = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	//回転---------------------------------------------------------------------
	//キーボードでの操作
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

	//減衰
	_angulvel *= _friction;

	//縦回転
	if (std::abs(_angulvel.x) > 0.001f &&
		!XMVector3Equal((worldZ), XMVectorZero())) {
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(worldZ, _angulvel.x);

		// 現在の回転クォータニオンに新しい回転を掛け合わせる
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//計算後のクォータニオンを入れる
		XMStoreFloat4(&GetQuaternion(), currentQuat);
		//プレイヤーのUpを更新
		GetComponent<Transform>()->Update();
	}
	//捻り回転
	if (std::abs(_angulvel.y) > 0.001f &&
		!XMVector3Equal((worldZ), XMVectorZero())) {
		//縦回転後のクォータニオンを再計算
		currentQuat = XMLoadFloat4(&GetQuaternion());

		//ワールドY軸（上方向）を基準にした回転クォータニオンを生成
		XMVECTOR yAxis = XMLoadFloat3(&GetUp());
		//クォータニオン回転
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, _angulvel.y);

		// 現在の回転クォータニオンに新しい回転を掛け合わせる
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//計算後のクォータニオンを入れる
		XMStoreFloat4(&GetQuaternion(), currentQuat);
		//プレイヤーのUpを更新
		GetComponent<Transform>()->Update();
	}
}

void Player_Tutorial::Draw(){
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//クォータニオンで回転
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

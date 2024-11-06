//Player.cpp
//20106_田中　蓮
//24_05_22
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Input.h"
#include "Player.h"
#include "C_AnimationModel.h"
#include "C_Transform.h"
#include "C_Collision.h"
#include "C_Audio.h"

const float Player::_FALL_NUM = -98.0f;
const float Player::_JUMP_POWER = 100.0f;
const float Player::_MAX_SPEED = 10.0f;
const float Player::_MOVE_NUM = 10.0f;
const float Player::_FRICTION_NUM = 2.0f;

Player::Player(const XMFLOAT3& pos, const XMFLOAT3& scl):
_state(nullptr){
	SetPosition(pos);
	SetScale(scl);
}

void Player::Init(){
	//Playerが持っていることを教える
	AddComponent<AnimationModel>(this);
	AddComponent<Collision>(this);
	AddComponent<Audio>(this);

	//コリジョン設定
	GetComponent<Collision>()->SetPosition(GetPosition());
	GetComponent<Collision>()->SetRotation(GetRotation());
	GetComponent<Collision>()->SetScale({ 3.0f, 7.0f, 2.0f });
	GetComponent<Collision>()->SetType(BOX);

	//モデル読み込み
	GetComponent<AnimationModel>()->Load("asset\\model\\bikeman.fbx");

	GetComponent<Audio>()->Load("asset\\audio\\jump.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\jump.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\splash01.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\splash01.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\splash02.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\splash02.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\splash03.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\splash03.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\bgm003.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\bgm003.wav", 0.1f);
	GetComponent<Audio>()->Play("asset\\audio\\bgm003.wav");

	//シェーダーセット
	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\NormalLightingVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\NormalLightingPS.cso");

	GetPosition().y = 216.5f;
	_accel.x = 25.0f;
	_accel.y = _FALL_NUM;
	_accel.z = 25.0f;
	_move = { 0.0f, 0.0f, 0.0f };
	SetRotation({ 0.0f, 0.0f, 0.0f });

	//Y軸周りに90度回転させてX方向を向かせる
	XMVECTOR yAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, XM_PIDIV2);
	XMStoreFloat4(&GetQuaternion(), rotationQuat);
}

void Player::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}

	_VertexShader->Release();
	_PixelShader->Release();
	_VertexLayout->Release();
}

void Player::Update() {
	Scene* scene;
	scene = Manager::GetScene();

	XMFLOAT3 oldPos = GetPosition();

	_move = { 0.0f, 0.0f, 0.0f };
	float dt = Time::GetDeltaTime();
	for (auto c : _components) {
		c->Update();
	}

	SetQuaternion(GetQuaternion());

	//速度の更新
	_velocity.x += _accel.x * _move.x * dt;
	_velocity.z += _accel.z * _move.z * dt;


	//摩擦
	_velocity.x -= _velocity.x * _FRICTION_NUM * dt;
	_velocity.z -= _velocity.z * _FRICTION_NUM * dt;


	//位置更新
	GetPosition() += _velocity * dt;
}

void Player::Draw(){
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//クォータニオンで回転
	rot = XMMatrixRotationQuaternion(XMLoadFloat4(&GetQuaternion()));
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	for (auto c : _components) {
		c->Draw();
	}
}

void Player::Hit(GameObject* obj){
}

void Player::PlayAudioBig() {
	PlayAudioMid();
	GetComponent<Audio>()->Play("asset\\audio\\splash03.wav", false);
}

void Player::PlayAudioMid() {
	PlayAudioSml();
	GetComponent<Audio>()->Play("asset\\audio\\splash02.wav", false);
}

void Player::PlayAudioSml() {
	GetComponent<Audio>()->Play("asset\\audio\\splash01.wav", false);
}

void Player::PlayAudioJump(){
	GetComponent<Audio>()->Play("asset\\audio\\jump.wav", false);
}

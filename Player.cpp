//Player.cpp
//20106_田中　蓮
//24_05_22
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "ShaderManager.h"
#include "Scene.h"
#include "Input.h"
#include "Player.h"
#include "ParticleSmoke.h"
#include "C_AnimationModel.h"
#include "C_Transform.h"
#include "C_Collision.h"
#include "C_Audio.h"
#include "C_Move.h"
#include "S_Title.h"
#include "S_Game.h"
#include "ParticleSmoke.h"

const float Player::FALL_NUM = -98.0f;
const float Player::JUMP_POWER = 100.0f;
const float Player::MAX_SPEED = 10.0f;
const float Player::MOVE_NUM = 10.0f;
const float Player::FRICTION_NUM = 2.0f;

Player::Player(const XMFLOAT3& pos, const XMFLOAT3& scl) :
	_state(nullptr) {
	SetPosition(pos);
	SetScale(scl);
}

void Player::Init() {
	//Playerが持っていることを教える
	AddComponent<AnimationModel>(this);
	AddComponent<Collision>(this);
	AddComponent<Audio>(this);
	AddComponent<Move>(this);

	//コリジョン設定
	GetComponent<Collision>()->SetPosition(GetPosition());
	GetComponent<Collision>()->SetRotation(GetRotation());
	GetComponent<Collision>()->SetScale({ 3.0f, 7.0f, 2.0f });
	GetComponent<Collision>()->SetType(ColiderType::BOX);

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


	_accel.x = 25.0f;
	_accel.y = FALL_NUM;
	_accel.z = 25.0f;
	_move = { 0.0f, 0.0f, 0.0f };
	SetRotation({ 0.0f, -XM_PIDIV4, 0.0f });

	/*
	//Y軸周りに90度回転させてX方向を向かせる
	XMVECTOR yAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, -XM_PIDIV4);
	XMStoreFloat4(&GetQuaternion(), rotationQuat);
	*/

	GetComponent<Transform>()->Update();

	_smoke = Manager::GetScene()->AddGameobject<Smoke>(Layer::Effect, XMFLOAT3(0.0f, 0.0f, 0.0f), 0.5f, XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));

	_smokepos = GetPosition() + (GetRight() * 0.3f) - (GetForward() * 0.75f) + (GetUp() * 4.5f);

	_smoke->SetPosition(_smokepos);
}

void Player::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}
}

void Player::Update() {
	Scene* scene = Manager::GetScene();
	if (scene) {
		Title* title = dynamic_cast<Title*>(scene);
		if (title) {
			return;
		}
	}

	if (Input::GetKeyTrigger(VK_SPACE)) {
		Manager::SetSceneFade<Game>(0.05f);
	}

	XMFLOAT3 oldpos = GetPosition();

	for (auto c : _components) {
		c->Update();
	}

	_smokepos = GetPosition() + (GetRight() * 0.37f) - (GetForward() * 1.4f) + (GetUp() * 4.3f);

	_smoke->SetPosition(_smokepos);
}

void Player::Draw() {
	Shader::SetShader(ShaderName::Normallit);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//クォータニオンで回転
	//rot = XMMatrixRotationQuaternion(XMLoadFloat4(&GetQuaternion()));
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world, _prevworld);

	for (auto c : _components) {
		c->Draw();
	}
}

void Player::Hit(GameObject* obj) {}

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

void Player::PlayAudioJump() {
	GetComponent<Audio>()->Play("asset\\audio\\jump.wav", false);
}

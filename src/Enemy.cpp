//Enemy.cpp
//20106_田中　蓮
//24_06_05
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "ShaderManager.h"
#include "Scene.h"
#include "Enemy.h"
#include "Player.h"
#include "Camera.h"
#include "Gaussian.h"
#include "EnemyBeheviorSequence.h"
#include "C_ModelRenderer.h"
#include "C_Transform.h"
#include "C_Collision.h"

//初期化処理=============================================================================
void Enemy::Init() {
	_components.push_back(new ModelRenderer(this));
	_components.push_back(new Collision(this));

	GetComponent<Collision>()->SetPosition(GetPosition());
	GetComponent<Collision>()->SetRotation(GetRotation());
	GetComponent<Collision>()->SetScale({5.0f, 10.0f, 5.0f});
	GetComponent<Collision>()->SetType(ColiderType::BOX);

	//モデル読み込み
	GetComponent<ModelRenderer>()->Load("asset\\model\\kuma.obj");


	_beheviorRoot = new EnemyBeheviorSequence(this);
	_beheviorRoot->AddChild(new BeheviorIdle(this))->SetUp();
	_beheviorRoot->AddChild(new BeheviorMove(this))->SetUp();
	_beheviorRoot->SetUp();
}

//終了処理===============================================================================
void Enemy::Uninit() {
	_beheviorRoot->Clean();
	delete _beheviorRoot;
}

//更新処理===============================================================================
void Enemy::Update() {
	_beheviorRoot->Update();

	GetComponent<Collision>()->SetPosition(GetPosition());
}

//描画処理===============================================================================
void Enemy::Draw() {
	Shader::SetShader(ShaderName::Unlit);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world, _prevworld);

	for (auto c : _components) {
		c->Draw();
	}
}

//衝突処理===============================================================================
void Enemy::Hit(GameObject* obj) {
	if (obj == nullptr) {
		return;
	}
}

void BeheviorIdle::SetUp() {
	_scene = Manager::GetScene();
	if (_scene) {
		_player = _scene->GetGameobject<Player>();
		_camera = _scene->GetGameobject<Camera>();
		_gaussian = Manager::GetGaussian();
	}
}

//プレイヤーを探す
BEHEVIOR_RESULT BeheviorIdle::Update() {
	_time += Time::GetDeltaTime();
	if (_time > 2.0f) {
		_time = 0.0f;
	}
	else{
		//return BEHEVIOR_RESULT::CONTINUE;
	}
	if (!_scene) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	if (!_player) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	if (!_camera) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	if (!_gaussian) {
		return BEHEVIOR_RESULT::FAILRE;
	}

	//プレイヤーとの距離を調べる
	XMFLOAT3 playerpos = _player->GetPosition();
	XMFLOAT3 dir = playerpos - _gameobject->GetPosition();
	
	float length = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;

	_camera->LerpFoV(1.0f, 0.8f);
	_gaussian->LerpDoF({ 0.1f, 0.1f }, 0.8f);
	_gaussian->LerpBoke(0.0f, 0.8f);
	//15いないなら近づく
	if (length < pow(30.0f, 2)) {
		return BEHEVIOR_RESULT::SUCCESS;
	}
	return BEHEVIOR_RESULT::CONTINUE;
}

void BeheviorMove::SetUp() {
	_scene = Manager::GetScene();
	if (_scene) {
		_player = _scene->GetGameobject<Player>();
		_camera = _scene->GetGameobject<Camera>();
		_gaussian = Manager::GetGaussian();
	}
}

//プレイヤーに近づく
BEHEVIOR_RESULT BeheviorMove::Update() {
	//プレイヤーとの距離を調べる
	if (!_scene) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	if (!_player) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	if (!_camera) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	if (!_gaussian) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	_camera->LerpFoV(XMConvertToRadians(55.0f), 0.4f);
	_gaussian->LerpDoF({ 0.15f, 0.36f }, 0.4f);
	_gaussian->LerpBoke(5.0f, 0.4f);

	XMFLOAT3 playerpos = _player->GetPosition();
	XMFLOAT3 dir = playerpos - _gameobject->GetPosition();
	//距離
	float length = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	float speed = 0.0f;

	//ターゲットの方向に回転
	_gameobject->GetRotation().y = atan2f(dir.x, dir.z);
	_gameobject->GetComponent<Transform>()->Update();
	speed = 10.0f;

	_gameobject->GetPosition() += _gameobject->GetForward() * speed * Time::GetDeltaTime();

	if (length > pow(45.0f, 2)) {
		return BEHEVIOR_RESULT::SUCCESS;
	}
	return BEHEVIOR_RESULT::CONTINUE;
}

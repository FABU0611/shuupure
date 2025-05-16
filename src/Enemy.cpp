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


	_beheviorRoot = new BeheviorSequence(this);
	_beheviorRoot->AddChild(new BeheviorIdle(this));
	_beheviorRoot->AddChild(new BeheviorMove(this));
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


BEHEVIOR_RESULT BeheviorIdle::Update() {
	_time += Time::GetDeltaTime();
	if (_time > 2.0f) {
		_time = 0.0f;
	}
	else{
		return BEHEVIOR_RESULT::CONTINUE;
	}

	//プレイヤーとの距離を調べる
	Scene* scene = Manager::GetScene();
	if (!scene) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	Player* player = scene->GetGameobject<Player>();
	if (!player) {
		return BEHEVIOR_RESULT::FAILRE;
	}

	XMFLOAT3 playerpos = player->GetPosition();
	XMFLOAT3 dir = playerpos - _enemy->GetPosition();
	//距離
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

	//15いないなら近づく
	if (length < 15.0f) {
		return BEHEVIOR_RESULT::SUCCESS;
	}
	return BEHEVIOR_RESULT::CONTINUE;
}

BEHEVIOR_RESULT BeheviorMove::Update() {
	//プレイヤーとの距離を調べる
	Scene* scene = Manager::GetScene();
	if (!scene) {
		return BEHEVIOR_RESULT::FAILRE;
	}
	Player* player = scene->GetGameobject<Player>();
	if (!player) {
		return BEHEVIOR_RESULT::FAILRE;
	}

	XMFLOAT3 playerpos = player->GetPosition();
	XMFLOAT3 dir = playerpos - _enemy->GetPosition();
	//距離
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	float speed = 0.0f;
	XMFLOAT3 vel = { 0.0f, 0.0f, 0.0f };;

	//5いないなら近づく
	vel = dir / length;
	_enemy->GetRotation().y = atan2f(dir.x, dir.z);
	_enemy->GetComponent<Transform>()->Update();
	speed = 10.0f;

	_enemy->GetPosition() += _enemy->GetForward() * speed * (1.0f / 60.0f);

	if (length > 30.0f) {
		return BEHEVIOR_RESULT::SUCCESS;
	}
	return BEHEVIOR_RESULT::CONTINUE;
}

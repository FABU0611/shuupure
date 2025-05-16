//Enemy.h
//20106_田中　蓮
//24_06_05
#pragma once

#include "GameObject.h"
#include "BeheviorNode.h"

class BeheviorSelector : public BeheviorNode {
public:
	using BeheviorNode::BeheviorNode;
	void SetUp()override {}
	BEHEVIOR_RESULT Update()override {}
};

class BeheviorIdle : public BeheviorNode {
	float _time = 0.0f;
	class Scene* _scene = nullptr;
	class Player* _player = nullptr;
	class Camera* _camera = nullptr;
	class Gaussian* _gaussian = nullptr;

public:
	using BeheviorNode::BeheviorNode;
	void SetUp()override;
	BEHEVIOR_RESULT Update()override;
};

class BeheviorMove : public BeheviorNode {
	class Scene* _scene = nullptr;
	class Player* _player = nullptr;
	class Camera* _camera = nullptr;
	class Gaussian* _gaussian = nullptr;

public:
	using BeheviorNode::BeheviorNode;
	void SetUp()override;
	BEHEVIOR_RESULT Update()override;
};


//敵のクラス=============================================================================
class Enemy : public GameObject {
	XMMATRIX _prevworld;
	BeheviorNode* _beheviorRoot;

public:
	Enemy() {}
	Enemy(const XMFLOAT3& pos, const XMFLOAT3& scl) {
		SetPosition(pos);
		SetScale(scl);
	}
	~Enemy() {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Hit(GameObject* obj)override;
};
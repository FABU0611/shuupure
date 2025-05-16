//Enemy.h
//20106_田中　蓮
//24_06_05
#pragma once

#include "GameObject.h"

class EnemyState;

enum class BEHEVIOR_RESULT {
	CONTINUE,	//継続
	SUCCESS,	//成功
	FAILRE,		//失敗
};

class BeheviorNode {
protected:
	std::vector<BeheviorNode*>	_child;
	class Enemy* _enemy;

public:
	BeheviorNode() = delete;
	BeheviorNode(Enemy* enemy)
		:_enemy(enemy) {}

	virtual BEHEVIOR_RESULT Update() = 0;

	void Clean() {
		for (auto& c : _child) {
			delete c;
		}
		_child.clear();
	}
	void AddChild(BeheviorNode* node) { _child.push_back(node); }

	template<typename T>
	T* AddChild() {}
};

class BeheviorSequence : public BeheviorNode {
	int	_index = 0;

public:
	//基本クラスのコンストラクタを利用
	using BeheviorNode::BeheviorNode;

	BEHEVIOR_RESULT Update()override {
		BEHEVIOR_RESULT result = _child[_index]->Update();

		switch (result) {
		case BEHEVIOR_RESULT::CONTINUE:
			return BEHEVIOR_RESULT::CONTINUE;

		case BEHEVIOR_RESULT::SUCCESS:
			if ((int)_child.size() != _index + 1) {
				_index++;
			}
			else {
				_index = 0;
			}
			if ((int)_child.size() < _index) {
				return BEHEVIOR_RESULT::CONTINUE;
			}
			else {
				return BEHEVIOR_RESULT::SUCCESS;
			}

		case BEHEVIOR_RESULT::FAILRE:
			_index = 0;
			return BEHEVIOR_RESULT::FAILRE;
		}
	}
};

class BeheviorSelector : public BeheviorNode {
public:
	using BeheviorNode::BeheviorNode;
	BEHEVIOR_RESULT Update()override {}
};

class BeheviorIdle : public BeheviorNode {
	float _time = 0.0f;
public:
	using BeheviorNode::BeheviorNode;
	BEHEVIOR_RESULT Update()override;
};

class BeheviorMove : public BeheviorNode {
public:
	using BeheviorNode::BeheviorNode;
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
//EnemyBeheviorSequence.h
//20106_田中　蓮
//25_05_16
#pragma once
#include "BeheviorNode.h"


class EnemyBeheviorSequence : public BeheviorNode {
	int	_index = 0;
	class Scene* _scene;
	class Camera* _camera;

public:
	//基本クラスのコンストラクタを利用
	using BeheviorNode::BeheviorNode;

	void SetUp() override {}
	BEHEVIOR_RESULT Update()override;
};
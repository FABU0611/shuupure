//EnemyBeheviorSequence.h
//20106_�c���@�@
//25_05_16
#pragma once
#include "BeheviorNode.h"


class EnemyBeheviorSequence : public BeheviorNode {
	int	_index = 0;
	class Scene* _scene;
	class Camera* _camera;

public:
	//��{�N���X�̃R���X�g���N�^�𗘗p
	using BeheviorNode::BeheviorNode;

	void SetUp() override {}
	BEHEVIOR_RESULT Update()override;
};
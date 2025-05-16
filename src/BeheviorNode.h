//BehevirorNode.h
//20106_�c���@�@
//25_05_16
#pragma once
#include <vector>

enum class BEHEVIOR_RESULT {
	CONTINUE,	//�p��
	SUCCESS,	//����
	FAILRE,		//���s
};

class BeheviorNode {
protected:
	std::vector<BeheviorNode*>	_child;
	class GameObject* _gameobject;

public:
	BeheviorNode() = delete;
	BeheviorNode(GameObject* gameobject)
		:_gameobject(gameobject) {}

	virtual void SetUp() = 0;
	virtual BEHEVIOR_RESULT Update() = 0;

	void Clean() {
		for (auto& c : _child) {
			delete c;
		}
		_child.clear();
	}
	BeheviorNode* AddChild(BeheviorNode* node) { 
		_child.push_back(node); 
		return node;
	}

	template<typename T>
	T* AddChild() {}
};
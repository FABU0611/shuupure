//TestObject.h
//20106_ìcíÜÅ@ò@
//25_01_16
#pragma once
#include "GameObject.h"

class TestObject : public GameObject {
	XMFLOAT3	_defpos{};
	float		_time{};

public:
	TestObject() {}
	TestObject(const XMFLOAT3& pos);
	~TestObject() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
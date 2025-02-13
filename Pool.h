//Pool.h	飛び込み台
//20106_田中　蓮
//24_09_07
#pragma once

#include "GameObject.h"

class Pool : public GameObject {
public:
	Pool(const XMFLOAT3& pos) {
		SetPosition(pos);
	}
	~Pool() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
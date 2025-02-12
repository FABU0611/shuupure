//Pool.h	��э��ݑ�
//20106_�c���@�@
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
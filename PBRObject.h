//PBRObject.h
//20106_�c���@�@
//25_01_16
#pragma once
#include "GameObject.h"

class PBRObject : public GameObject {
	XMFLOAT4	_param{};
	ID3D11ShaderResourceView* _envtex{};

public:
	PBRObject() {}
	~PBRObject() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
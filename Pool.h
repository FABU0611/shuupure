//Pool.h	��э��ݑ�
//20106_�c���@�@
//24_09_07
#pragma once

#include "GameObject.h"

class Pool : public GameObject {
	ID3D11VertexShader* _vertexshader;
	ID3D11PixelShader* _pixelshader;
	ID3D11InputLayout* _vertexlayout;

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
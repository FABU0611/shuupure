//Pool.h	îÚÇ—çûÇ›ë‰
//20106_ìcíÜÅ@ò@
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
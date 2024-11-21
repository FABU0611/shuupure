//sky.h
//20106_ìcíÜÅ@ò@
//24_06_05
#pragma once

#include "GameObject.h"

class Sky : public GameObject {
	ID3D11VertexShader*	_vertexshader{};
	ID3D11PixelShader*	_pixelshader{};
	ID3D11InputLayout*	_vertexlayout{};

	XMMATRIX			_prevworld{};

public:
	Sky(const float& scl){
		SetScale({ scl, scl,scl });
	}
	~Sky() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
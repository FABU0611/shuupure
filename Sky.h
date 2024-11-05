//sky.h
//20106_ìcíÜÅ@ò@
//24_06_05
#pragma once

#include "GameObject.h"

class Sky : public GameObject {
	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;

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
//Skyobj.h
//20106_ìcíÜÅ@ò@
//24_09_17
#pragma once

#include "GameObject.h"

class Skyobj : public GameObject {
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _vertexLayout;

public:
	Skyobj(const float& scl){
		SetScale({ scl, scl,scl });
	}
	~Skyobj() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
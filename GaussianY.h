//Gaussian.h
//20106_田中　蓮
//24_11_07
#pragma once
#include "Main.h"

class GaussianY {
	ID3D11Buffer* _vertexbuffer = NULL;

	ID3D11VertexShader* _vertexshader{};	//頂点シェーダーオブジェクトポインタ
	ID3D11PixelShader* _pixelshader{};	//ピクセルシェーダーオブジェクトポインタ
	ID3D11InputLayout* _vertexlayout{};	//頂点レイアウトオブジェクト

public:
	GaussianY() {}
	~GaussianY() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
//DivingBoard.h	飛び込み台
//20106_田中　蓮
//24_09_07
#pragma once

#include "GameObject.h"

class DivingBoard : public GameObject {
	ID3D11VertexShader* _vertexshader{};
	ID3D11PixelShader*	_pixelshader{};
	ID3D11InputLayout*	_vertexlayout{};

public:
	DivingBoard(const XMFLOAT3& pos) {
		SetPosition(pos);
	}
	~DivingBoard() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
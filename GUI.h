//GUI.h
//20106_田中　蓮
//24_11_14
#pragma once

#include "GameObject.h"

class GUI : public GameObject{
protected:
	ID3D11VertexShader*	_vertexshader{};
	ID3D11PixelShader*	_pixelshader{};
	ID3D11InputLayout*	_vertexlayout{};

	XMFLOAT3			_size{};
	const wchar_t*		_texname{};

public:
	GUI() {};
	virtual ~GUI() {}


	virtual bool IsHovered(const POINT pos) = 0;	//マウスが重なっているか
};
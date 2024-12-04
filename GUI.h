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
	GUI(const XMFLOAT3& pos, const XMFLOAT3& size, const wchar_t* texname);
	virtual ~GUI() {}

	virtual void Init();
	virtual void Uninit();
	virtual void Update() override{}
	virtual void Draw();

	virtual bool IsHovered(const POINT pos);	//マウスが重なっているか
	virtual bool OnClicked(const int& mousebutton) = 0;
};
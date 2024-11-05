//Polygon2D.h
//20106_田中　蓮
//2024_04_24
#pragma once

#include "GameObject.h"

class Polygon2D : public GameObject{
	ID3D11VertexShader*			_VertexShader;
	ID3D11PixelShader*			_PixelShader;
	ID3D11InputLayout*			_VertexLayout;

	float						_rot{};

	const wchar_t*				_texname;		//画像ファイルのパス
	XMFLOAT3					_texpos[4];
public:
	Polygon2D(const XMFLOAT3& pos, const XMFLOAT3& size, const wchar_t* name);
	~Polygon2D() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
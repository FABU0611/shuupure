//field.h
//20106_田中　蓮
//2024_05_08
#pragma once
#include "GameObject.h"
#include "C_Sprite3D.h"
#include "Renderer.h"

class Water : public GameObject{
	ID3D11Buffer*				_vertexbuffer = NULL;
	ID3D11ShaderResourceView*	_texture = NULL;

	ID3D11VertexShader*			_VertexShader;
	ID3D11PixelShader*			_PixelShader;
	ID3D11InputLayout*			_VertexLayout;

	XMFLOAT3					_size;
	const wchar_t*				_texname;		//画像ファイルのパス

public:
	Water(const XMFLOAT3& size):
	_size(size), _texname(L"asset\\texture\\fade.png"){
		SetScale({ size.x, size.y, size.z });
		AddComponent<Sprite3D>(this);
		GetComponent<Sprite3D>()->LoadTexture(_texname);
		GetComponent<Sprite3D>()->SetTangent({ 1.0f, 0.0f, 0.0f });
		GetComponent<Sprite3D>()->SetNormal({ 0.0f, 0.0f, -1.0f });
		GetComponent<Sprite3D>()->LoadNormalTexture(L"asset\\texture\\water_normal.png");
		GetComponent<Sprite3D>()->SetDrawMode(DrawMode3D::Normal);
	}
	~Water() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
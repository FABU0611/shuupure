//MeshField.h
//20106_田中　蓮
//24_09_17
#pragma once

#include "GameObject.h"
#include "Renderer.h"

struct ShaderPack;

class MeshField : public GameObject {
	static const int			VERTEX_NUM = 42;

	ID3D11Buffer*				_vertexbuffer = NULL;
	ID3D11Buffer*				_indexbuffer = NULL;
	ID3D11ShaderResourceView*	_texture = NULL;

	ShaderPack*					_shader{};

	VERTEX_3D					_vertex[VERTEX_NUM][VERTEX_NUM];

	float						_size;
	const wchar_t*				_texname;		//画像ファイルのパス

public:
	MeshField() :
		_size(1.0f), _texname(L"asset\\texture\\field_mip.dds") {
		SetScale({ 1.0f, 1.0f, 1.0f });
	}
	~MeshField() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	float GetHeight(const XMFLOAT3& pos);
};
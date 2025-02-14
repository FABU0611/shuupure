//Wave.h
//20106_田中　蓮
//24_11_11
#pragma once

#include "GameObject.h"
#include "Renderer.h"

class Wave : public GameObject {
	static const int			VERTEX_NUM = 42;
	static const float			WAVE_AMPLITUDE;					// ウェーブポイントの振幅
	static const float			WAVE_LENGTH;					// ウェーブポイントの波長
	static const float			WAVE_CYCLE;						// ウェーブポイントの周期

	ID3D11Buffer*				_vertexbuffer = NULL;
	ID3D11Buffer*				_indexbuffer = NULL;
	ID3D11ShaderResourceView*	_texture = NULL;
	ID3D11ShaderResourceView*	_normaltexture = NULL;
	ID3D11ShaderResourceView*	_envtexture = NULL;

	VERTEX_3D					_vertex[VERTEX_NUM][VERTEX_NUM];

	XMFLOAT4					_param{};
	float						_size{};

	float						_time{};

public:
	Wave() :
		_size(1.0f) {
		SetScale({ 1.0f, 1.0f, 1.0f });
	}
	~Wave() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
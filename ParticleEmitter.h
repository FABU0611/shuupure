//ParticleEmitter.h		パーティクル発射装置
//20106_田中　蓮
//24_07_05
#pragma once

#include "GameObject.h"

class ParticleEmitter : public GameObject {
	ID3D11Buffer*				_vertexbuffer = NULL;
	ID3D11ShaderResourceView*	_texture = NULL;

	const wchar_t*		_texname{};		//画像ファイルのパス

	//生成するパーティクル
	struct PARTICLE{
		XMFLOAT3	Pos;	//位置
		XMFLOAT3	Size;	//おおきさ
		XMFLOAT3	Defsize;	//元のおおきさ
		XMFLOAT4	Color;	//色
		XMFLOAT3	Vel;	//速度
		XMFLOAT3	Axel;	//加速度
		bool		Enable;	//使っているか
		float		Life;	//ライフ(秒)
	};
	static const int	PARTICLE_MAX = 1000;

protected:
	PARTICLE			_particle[PARTICLE_MAX]{};
	float				_size{};
	XMFLOAT4			_color{};

public:
	ParticleEmitter() {}
	ParticleEmitter(const XMFLOAT3& pos, const float& size, const XMFLOAT4& color, const wchar_t* texture) :
		_size(size), _texname(texture), _color(color) {
		SetPosition(pos);
		for (int i = 0; i < PARTICLE_MAX; i++) {
			_particle[i].Defsize = { size,size,size };
			_particle[i].Color = color;
		}
	}
	~ParticleEmitter() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
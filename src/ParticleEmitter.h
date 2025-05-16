//ParticleEmitter.h		パーティクル発射装置
//20106_田中　蓮
//24_07_05
#pragma once

#include "GameObject.h"

class Sprite3D;

class ParticleEmitter : public GameObject {
	static const int			PARTICLE_MAX = 1000;

	//生成するパーティクル
	struct PARTICLE{
		XMFLOAT3	Pos;	//位置
		XMFLOAT3	Size;	//おおきさ
		XMFLOAT4	Color;	//色
		XMFLOAT3	Vel;	//速度
		XMFLOAT3	Axel;	//加速度
		bool		Enable;	//使っているか
		float		Life;	//ライフ(秒)
	};
	struct PARTICLE_INSTANCE {
		XMFLOAT3	Pos;
		float		Dummy;
		XMFLOAT3	Size;
		float		Dummy2;
		XMFLOAT4	Color;
	};

	const wchar_t*		_texname{};		//画像ファイルのパス

	ID3D11Buffer*		_particlebuffer{};
	ID3D11ShaderResourceView*	_particleSRV{};
	PARTICLE_INSTANCE*	_particleinstancedata{};
	int					_instancecount{};

	friend class Sprite3D;

	void UpdateParticle();

protected:
	PARTICLE			_particle[PARTICLE_MAX]{};
	float				_defsize{};		//デフォルトの大きさ
	XMFLOAT4			_defcolor{};		//デフォルトの色

public:
	ParticleEmitter() {}
	ParticleEmitter(const XMFLOAT3& pos, const float& size, const XMFLOAT4& color, const wchar_t* texture) :
		_defsize(size), _texname(texture), _defcolor(color) {
		SetPosition(pos);
		for (int i = 0; i < PARTICLE_MAX; i++) {
			_particle[i].Pos = pos;
			_particle[i].Size = { size,size,size };
			_particle[i].Color = color;
		}
	}
	~ParticleEmitter() {}

	void Init()override;
	void Uninit()override;
	void Update()override {}
	void Draw()override;
};
//ParticleEmitter.h		�p�[�e�B�N�����ˑ��u
//20106_�c���@�@
//24_07_05
#pragma once

#include "GameObject.h"

class Sprite3D;

class ParticleEmitter : public GameObject {
	static const int			PARTICLE_MAX = 1000;

	//��������p�[�e�B�N��
	struct PARTICLE{
		XMFLOAT3	Pos;	//�ʒu
		XMFLOAT3	Size;	//��������
		XMFLOAT4	Color;	//�F
		XMFLOAT3	Vel;	//���x
		XMFLOAT3	Axel;	//�����x
		bool		Enable;	//�g���Ă��邩
		float		Life;	//���C�t(�b)
	};
	struct PARTICLE_INSTANCE {
		XMFLOAT3	Pos;
		float		Dummy;
		XMFLOAT3	Size;
		float		Dummy2;
		XMFLOAT4	Color;
	};

	const wchar_t*		_texname{};		//�摜�t�@�C���̃p�X

	ID3D11Buffer*		_particlebuffer{};
	ID3D11ShaderResourceView*	_particleSRV{};
	PARTICLE_INSTANCE*	_particleinstancedata{};
	int					_instancecount{};

	friend class Sprite3D;

	void UpdateParticle();

protected:
	PARTICLE			_particle[PARTICLE_MAX]{};
	float				_defsize{};		//�f�t�H���g�̑傫��
	XMFLOAT4			_defcolor{};		//�f�t�H���g�̐F

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
//ParticleEmitter.h		�p�[�e�B�N�����ˑ��u
//20106_�c���@�@
//24_07_05
#pragma once

#include "GameObject.h"

class ParticleEmitter : public GameObject {
	ID3D11Buffer*				_vertexbuffer = NULL;
	ID3D11ShaderResourceView*	_texture = NULL;

	const wchar_t*		_texname{};		//�摜�t�@�C���̃p�X

	//��������p�[�e�B�N��
	struct PARTICLE{
		XMFLOAT3	Pos;	//�ʒu
		XMFLOAT3	Size;	//��������
		XMFLOAT3	Defsize;	//���̂�������
		XMFLOAT4	Color;	//�F
		XMFLOAT3	Vel;	//���x
		XMFLOAT3	Axel;	//�����x
		bool		Enable;	//�g���Ă��邩
		float		Life;	//���C�t(�b)
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
//ParticleFireworks.h		�p�[�e�B�N�����ˑ��u
//20106_�c���@�@
//24_07_05
#pragma once

#include "ParticleEmitter.h"
#include <random>

class Fireworks : public ParticleEmitter {
	static const int	PARTICLE_MAX = 500;
	float				_volume{};
	XMFLOAT3			_befcolor{};

public:
	Fireworks(const XMFLOAT3& pos, const float& size, const XMFLOAT4& color, const float& volume):
	ParticleEmitter(pos, size, color, L"asset\\texture\\particle.png"), _volume(volume){
		_befcolor.x = color.z;
		_befcolor.y = color.x;
		_befcolor.z = color.y;
	}
	~Fireworks() {}

	void Init()override {
		ParticleEmitter::Init();
		//��������
		std::random_device rd;
		//�͈͂��w��
		std::uniform_real_distribution<float> dist(-150.0f, 150.0f);

		//�p�[�e�B�N���𔭎�
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (_particle[i].Enable) {
				continue;
			}
			for (int j = 0; j < PARTICLE_MAX; j++) {
				_particle[i + j].Enable = true;
				_particle[i + j].Life = 1.0f;
				_particle[i + j].Pos = GetPosition();
				_particle[i + j].Vel = { dist(rd), dist(rd) + 75.0f, dist(rd) };

				// �x�N�g���̐��K��
				_particle[i + j].Vel = VectorNormalize(_particle[i + j].Vel) * _volume;

				_particle[i + j].Axel.x = 0.0f;
				_particle[i + j].Axel.y = -98.0f;
				_particle[i + j].Axel.z = 0.0f;

				_particle[i + j].Size = { _defsize, _defsize, _defsize };
			}
			break;
		}
	}
	void Update()override {
		float dt = 1.0f / 60.0f;


		//�p�[�e�B�N�����ړ�
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (!_particle[i].Enable) {
				continue;
			}
			_particle[i].Vel += _particle[i].Axel * dt;
			_particle[i].Vel *= 0.95f;
			_particle[i].Pos += _particle[i].Vel * dt;

			_particle[i].Life -= dt;

			if (_particle[i].Life < 0.5f) {
				_particle[i].Color = { _befcolor.x, _befcolor.y, _befcolor.z, 1.0f };
			}

			_particle[i].Size -= 1.0f * dt;

			_particle[i].Size.x = std::max(_particle[i].Size.x, 0.0f);
			_particle[i].Size.y = std::max(_particle[i].Size.y, 0.0f);
			_particle[i].Size.z = std::max(_particle[i].Size.z, 0.0f);

			//���Ŕ���
			if (_particle[i].Pos.y <= 0.0f) {
				_particle[i].Vel.y *= -1;
			}
			//���ԂŖ����ɂ���
			if (_particle[i].Size.x <= 0) {
				_particle[i].Enable = false;
			}
		}
	}
};
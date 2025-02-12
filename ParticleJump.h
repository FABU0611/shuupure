//ParticleJump.h
//20106_�c���@�@
//24_09_16
#pragma once

#include "ParticleEmitter.h"
#include <random>

class Shockwave : public ParticleEmitter {
	static const int	PARTICLE_MAX = 500;

public:
	Shockwave(const XMFLOAT3& pos, const float& size, const XMFLOAT4& color):
	ParticleEmitter(pos, size, color, L"asset\\texture\\particle.png"){}
	~Shockwave() {}

	void Init()override {
		ParticleEmitter::Init();
		//��������
		std::random_device rd;
		//�͈͂��w��
		std::uniform_real_distribution<float> dist(-25.0f, 25.0f);

		//�p�[�e�B�N���𔭎�
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (_particle[i].Enable) {
				continue;
			}
			for (int j = 0; j < PARTICLE_MAX; j++) {
				_particle[i + j].Enable = true;
				_particle[i + j].Life = 1.0f;
				_particle[i + j].Pos = GetPosition();
				_particle[i + j].Pos.y -= 3.5f;
				_particle[i + j].Vel = { dist(rd), 0.0f, dist(rd) };

				// �x�N�g���̐��K��
				_particle[i + j].Vel = VectorNormalize(_particle[i + j].Vel) * 50.0f;

				_particle[i + j].Axel.x = 0.0f;
				_particle[i + j].Axel.y = 0.0f;
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
				_particle[i].Color = { 0.5f, 0.2f, 0.2f, 1.0f };
			}

			_particle[i].Size -= 1.0f * dt;

			_particle[i].Size.x = std::max(_particle[i].Size.x, 0.0f);
			_particle[i].Size.y = std::max(_particle[i].Size.y, 0.0f);
			_particle[i].Size.z = std::max(_particle[i].Size.z, 0.0f);

			//���ԂŖ����ɂ���
			if (_particle[i].Size.x <= 0) {
				_particle[i].Enable = false;
			}
		}
	}
};
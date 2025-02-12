//ParticleSmoke.h		��
//20106_�c���@�@
//24_12_12
#pragma once

#include "ParticleEmitter.h"
#include "Manager.h"
#include <random>
#include "Time.h"

class Smoke : public ParticleEmitter {
	static const int	PARTICLE_MAX = 250;

	float _time{};
public:
	Smoke(const XMFLOAT3& pos, const float& size, const XMFLOAT4& color):
	ParticleEmitter(pos, size, color, L"asset\\texture\\particle.png"){}
	~Smoke() {}

	void Update()override {
		//��������
		std::random_device rd;
		//�͈͂��w��
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

		float dt = Time::GetDeltaTime();

		//�p�[�e�B�N���𔭎�
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (_particle[i].Enable) {
				_particle[i].Vel += _particle[i].Axel * dt;
				_particle[i].Vel *= 0.9f;
				_particle[i].Pos += _particle[i].Vel * dt;

				_particle[i].Life -= dt;

				//���ԂŖ����ɂ���
				if (_particle[i].Life <= 0) {
					_particle[i].Enable = false;
				}
				continue;
			}
			else if (_time > 0.1f) {
				_particle[i].Enable = true;
				_particle[i].Life = 3.0f;
				_particle[i].Pos = GetPosition();
				_particle[i].Vel = { dist(rd), dist(rd) + 1.0f, dist(rd) };
				_particle[i].Axel = { 0.0f, 5.0f, 0.0f };
				_particle[i].Size = { _defsize, _defsize, _defsize };
				_particle[i].Color = _defcolor;
				_time = 0.0f;
				break;
			}
		}
		_time += dt;
	}
};
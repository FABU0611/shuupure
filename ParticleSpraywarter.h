//ParticleSprayarter.h		�����Ԃ�
//20106_�c���@�@
//24_08_23
#pragma once

#include "ParticleEmitter.h"
#include "Manager.h"
#include <random>
#include "Time.h"

class SprayWater : public ParticleEmitter {
	static const int	PARTICLE_MAX = 500;

	float				_spraysize{};

public:
	SprayWater(const XMFLOAT3& pos, const float& size, const float& spraysize):
	ParticleEmitter(pos, size, { 0.2f, 0.2f, 0.6f, 1.0f }, L"asset\\texture\\particle.png"), _spraysize(spraysize){}
	~SprayWater() {}

	void Init()override {
		ParticleEmitter::Init();
		//��������
		std::random_device rd;
		//�͈͂��w��
		std::uniform_real_distribution<float> dist(-150.0f, 150.0f);		//���a
		std::uniform_real_distribution<float> offsetDist(-10.0f, 10.0f);//�ʒu�Ƀ����_������ǉ�
		std::uniform_real_distribution<float> speedDist(0.0f, 10.0f);	//���x�͈͂𒲐�

		//�p�[�e�B�N���𔭎�
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (_particle[i].Enable) {
				continue;
			}
			//������
			for (int j = 0; j < PARTICLE_MAX; j++) {
				_particle[i + j].Enable = true;
				_particle[i + j].Life = 5.0f;

				//�����_���ȕ����x�N�g���𐶐�
				XMFLOAT3 offsetpos = { offsetDist(rd), offsetDist(rd), offsetDist(rd) };
				offsetpos = VectorNormalize(offsetpos);

				//�����ʒu�𒲐�
				_particle[i + j].Pos = GetPosition() + offsetpos * offsetDist(rd);
				_particle[i + j].Pos.y += 10.0f;

				//���S����̋���
				float centerdistance = std::sqrtf(offsetpos.x * offsetpos.x + offsetpos.z * offsetpos.z);

				//���S����̋����ō�����ݒ�
				float ndistance = std::min(centerdistance, 1.0f);
				float height = 1.0f * ndistance;

				_particle[i + j].Vel = { dist(rd) * (centerdistance * 10.0f) , 0.0f, dist(rd) * (centerdistance * 10.0f) };

				_particle[i + j].Vel = VectorNormalize(_particle[i + j].Vel) * height * _spraysize;
				_particle[i + j].Vel.y = (speedDist(rd) * height);

				_particle[i + j].Axel.x = 0.0f;
				_particle[i + j].Axel.y = -98.0f;
				_particle[i + j].Axel.z = 0.0f;
				_particle[i + j].Size = _particle[i + j].Defsize * _spraysize;
			}
			break;
		}
	}
	void Uninit()override { ParticleEmitter::Uninit(); }
	void Update()override {
		float dt = Time::GetDeltaTime();

		//�p�[�e�B�N�����ړ�
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (!_particle[i].Enable) {
				continue;
			}
			_particle[i].Vel += _particle[i].Axel * dt;
			_particle[i].Vel *= 0.99f;
			_particle[i].Pos += _particle[i].Vel * dt;

			_particle[i].Life -= dt;

			if (_particle[i].Life < 2.5f) {
				_particle[i].Color = { 0.2f, 0.2f, 0.2f, _particle[i].Life / 5 };
			}

			if (_particle[i].Pos.y < 0.0f) {
				_particle[i].Pos.y = 0.0f;
			}

			//���ԂŖ����ɂ���
			if (_particle[i].Life <= 0) {
				_particle[i].Enable = false;
			}
		}
	}
	void Draw()override { ParticleEmitter::Draw(); }
};
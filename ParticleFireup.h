//ParticleFireup.h		�ԉ΂̔����O�̂��
//20106_�c���@�@
//24_08_23
#pragma once

#include "ParticleEmitter.h"
#include "ParticleFireworks.h"
#include "Manager.h"
#include <random>

class Fireup : public ParticleEmitter {
	static const int	PARTICLE_MAX = 500;

	float				_stoptime{};
	bool				_isSpawn = false;
	float				_volume{};
	float				_size{};

public:
	Fireup(const XMFLOAT3& pos, const float& size, const float& stoptime, const XMFLOAT4& color, const float& volume):
	ParticleEmitter(pos, size, color, L"asset\\texture\\particle.png"), _size(size), _stoptime(stoptime), _volume(volume){}
	~Fireup() {}

	void Init()override { ParticleEmitter::Init(); }
	void Uninit()override { ParticleEmitter::Uninit(); }
	void Update()override {
		//��������
		std::random_device rd;
		//�͈͂��w��
		std::uniform_real_distribution<float> dist(-10.0f, 10.0f);

		float dt = 1.0f / 60.0f;

		//�㏸
		GetPosition().y += 1.0f;

		if (_stoptime < 0.0f) {
			_stoptime = 0.0f;
		}
		//�������X�|�[��������
		else if (!_isSpawn) {
			_stoptime -= dt;
			//�p�[�e�B�N���𔭎�
			for (int i = 0; i < PARTICLE_MAX; i++) {
				if (_particle[i].Enable) {
					continue;
				}
				_particle[i].Enable = true;
				_particle[i].Life = 1.0f;
				_particle[i].Pos = GetPosition();
				_particle[i].Vel = { dist(rd), dist(rd) + 5.0f, dist(rd) };
				_particle[i].Axel.x = 0.0f;
				_particle[i].Axel.y = -98.0f;
				_particle[i].Axel.z = 0.0f;
				_particle[i].Size = _particle[i].Defsize;
				break;
			}
		}
		if (!_isSpawn && _stoptime == 0.0f) {
			Scene* scene;
			scene = Manager::GetScene();

			scene->AddGameobject<Fireworks>(Effect, GetPosition(), _size, XMFLOAT4(0.2f, 0.2f, 0.5f, 1.0f), _volume);
			_isSpawn = true;
		}

		//�p�[�e�B�N�����ړ�
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (!_particle[i].Enable) {
				continue;
			}
			_particle[i].Vel += _particle[i].Axel * dt;
			_particle[i].Vel *= 0.9f;
			_particle[i].Pos += _particle[i].Vel * dt;

			_particle[i].Life -= dt;
			_particle[i].Size -= 2.0f * dt;

			_particle[i].Size.x = std::max(_particle[i].Size.x, 0.0f);
			_particle[i].Size.y = std::max(_particle[i].Size.y, 0.0f);
			_particle[i].Size.z = std::max(_particle[i].Size.z, 0.0f);

			//���Ŕ���
			if (_particle[i].Pos.y <= 0.0f) {
				_particle[i].Vel.y *= -1;
			}
			//���ԂŖ����ɂ���
			if (_particle[i].Life <= 0) {
				_particle[i].Enable = false;
			}
		}
	}
	void Draw()override { ParticleEmitter::Draw(); }
};
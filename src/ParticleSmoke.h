//ParticleSmoke.h		煙
//20106_田中　蓮
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
		//乱数生成
		std::random_device rd;
		//範囲を指定
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

		float dt = Time::GetDeltaTime();

		//パーティクルを発射
		for (int i = 0; i < PARTICLE_MAX; i++) {
			if (_particle[i].Enable) {
				_particle[i].Vel += _particle[i].Axel * dt;
				_particle[i].Vel *= 0.9f;
				_particle[i].Pos += _particle[i].Vel * dt;

				_particle[i].Life -= dt;

				//時間で無効にする
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
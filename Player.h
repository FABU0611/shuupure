//Player.h
//20106_田中　蓮
//24_05_22
#pragma once

#include "GameObject.h"
#include "Time.h"

class PlayerState;
class Smoke;

class Player : public GameObject {
	PlayerState*		_state{};

	XMFLOAT3			_velocity{};
	XMFLOAT3			_accel{};
	XMFLOAT3			_move{};

	XMFLOAT3			_smokepos{};
	Smoke*				_smoke{};

	static const float FALL_NUM;
	static const float JUMP_POWER;
	static const float MAX_SPEED;
	static const float MOVE_NUM;
	static const float FRICTION_NUM;
public:
	Player(const XMFLOAT3& pos, const XMFLOAT3& scl);
	~Player() {
		if (_state) {
			delete _state;
		}
	}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Hit(GameObject* obj);

	XMFLOAT3& GetAccel() { return _accel; }
	XMFLOAT3& GetVelocity() { return _velocity; }

	const void SetVelocity(const XMFLOAT3& vel) { _velocity = vel; }

	void Fall(const float& deltatime) { _velocity.y += _accel.y * deltatime; }
	void ChangeState(PlayerState* next) {
		if (_state) {
			delete _state;
			_state = next;
		}
	}
	PlayerState* GetPlayerState()const { return _state; }
	void TrunHeadup() {
		XMFLOAT4 headup = { 0.0f, 0.0f, 0.0f, 1.0f };
		
		XMVECTOR newq = XMQuaternionSlerp(XMLoadFloat4(&GetQuaternion()), XMLoadFloat4(&headup), Time::GetDeltaTime() * 2.0f);
		XMStoreFloat4(&GetQuaternion(), newq);
	}
	void PlayAudioBig();
	void PlayAudioMid();
	void PlayAudioSml();
	void PlayAudioJump();
};

/*
*セーブする際、シリアライズをはさむことでマルチプラットフォームに対応
Playerとか
↓
シリアライズ	
↓
ファイル

・データフォーマット
　・バイナリ
　・XML			テキスト
　・Json		テキスト
*/
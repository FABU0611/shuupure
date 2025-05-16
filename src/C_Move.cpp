//C_Move.cpp
//20106_田中　蓮
//25_02_13
#include "Main.h"
#include "C_Move.h"
#include "Time.h"
#include "GameObject.h"
#include "Manager.h"

const float Move::MOVE_NUM = 10.0f;

void Move::Update() {
	_move = { 0.0f, 0.0f, 0.0f };
	float dt = Time::GetDeltaTime();

	//移動---------------------------------------------------------------------
	if (Input::GetKeyPress(_forwardkey)) {
		_move += _gameobject->GetForward() * 1.0f;
	}
	if (Input::GetKeyPress(_leftkey)) {
		_move -= _gameobject->GetRight() * 1.0f;
	}
	if (Input::GetKeyPress(_backkey)) {
		_move -= _gameobject->GetForward() * 1.0f;
	}
	if (Input::GetKeyPress(_rightkey)) {
		_move += _gameobject->GetRight() * 1.0f;
	}


	//斜め移動の速度を正規化して調整
	//float magnitude = std::sqrt(_move.x * _move.x + _move.z * _move.z);
	//if (magnitude > 0.0f) {
	//	_move.x /= magnitude;
	//	_move.z /= magnitude;
	//	//1.0f超えたら1.0f
	//	magnitude = std::min(magnitude, 1.0f);
	//	_move.x *= _MOVE_NUM * magnitude;
	//	_move.z *= _MOVE_NUM * magnitude;
	//}


	//斜め移動で早くならないように正規化
	if (!XMVector3Equal(XMLoadFloat3(&_move), XMVectorZero())) {
		_move = VectorNormalize(_move) * MOVE_NUM;
	}

	//速度の更新
	_velocity.x += _accel.x * _move.x * dt;
	_velocity.z += _accel.z * _move.z * dt;

	//摩擦
	_velocity.x -= _velocity.x * _friction * dt;
	_velocity.z -= _velocity.z * _friction * dt;

	//位置更新
	_gameobject->GetPosition() += _velocity * dt;
}

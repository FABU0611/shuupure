//C_Move.cpp
//20106_“c’†@˜@
//25_02_13
#include "Main.h"
#include "C_Move.h"
#include "Time.h"
#include "GameObject.h"

const float Move::MOVE_NUM = 10.0f;

void Move::Update() {
	_move = { 0.0f, 0.0f, 0.0f };
	float dt = Time::GetDeltaTime();

	//ˆÚ“®---------------------------------------------------------------------
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


	//ŽÎ‚ßˆÚ“®‚Ì‘¬“x‚ð³‹K‰»‚µ‚Ä’²®
	//float magnitude = std::sqrt(_move.x * _move.x + _move.z * _move.z);
	//if (magnitude > 0.0f) {
	//	_move.x /= magnitude;
	//	_move.z /= magnitude;
	//	//1.0f’´‚¦‚½‚ç1.0f
	//	magnitude = std::min(magnitude, 1.0f);
	//	_move.x *= _MOVE_NUM * magnitude;
	//	_move.z *= _MOVE_NUM * magnitude;
	//}


	//ŽÎ‚ßˆÚ“®‚Å‘‚­‚È‚ç‚È‚¢‚æ‚¤‚É³‹K‰»
	if (!XMVector3Equal(XMLoadFloat3(&_move), XMVectorZero())) {
		_move = VectorNormalize(_move) * MOVE_NUM;

		//ˆÚ“®ƒxƒNƒgƒ‹‚©‚ç‰ñ“]Šp“x‚ðŒvŽZ
		float angle = std::atan2f(_move.z, -_move.x) + (XM_PI * 1.5f);

		//‰ñ“]Šp“x‚ðƒ‚ƒfƒ‹‚Ì‰ñ“]‚ÉÝ’è
		_gameobject->SetRotation({ _gameobject->GetRotation().x, angle, _gameobject->GetRotation().z }); // YŽ²Žü‚è‚Ì‰ñ“]‚ðÝ’è
		//Forward‚ðXV
		_gameobject->GetComponent<Transform>()->Update();
	}

	//‘¬“x‚ÌXV
	_velocity.x += _accel.x * _move.x * dt;
	_velocity.z += _accel.z * _move.z * dt;

	//–€ŽC
	_velocity.x -= _velocity.x * _friction * dt;
	_velocity.z -= _velocity.z * _friction * dt;

	//ˆÊ’uXV
	_gameobject->GetPosition() += _velocity * dt;
}

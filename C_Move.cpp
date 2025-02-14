//C_Move.cpp
//20106_�c���@�@
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

	//�ړ�---------------------------------------------------------------------
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


	//�΂߈ړ��̑��x�𐳋K�����Ē���
	//float magnitude = std::sqrt(_move.x * _move.x + _move.z * _move.z);
	//if (magnitude > 0.0f) {
	//	_move.x /= magnitude;
	//	_move.z /= magnitude;
	//	//1.0f��������1.0f
	//	magnitude = std::min(magnitude, 1.0f);
	//	_move.x *= _MOVE_NUM * magnitude;
	//	_move.z *= _MOVE_NUM * magnitude;
	//}


	//�΂߈ړ��ő����Ȃ�Ȃ��悤�ɐ��K��
	if (!XMVector3Equal(XMLoadFloat3(&_move), XMVectorZero())) {
		_move = VectorNormalize(_move) * MOVE_NUM;
	}

	//���x�̍X�V
	_velocity.x += _accel.x * _move.x * dt;
	_velocity.z += _accel.z * _move.z * dt;

	//���C
	_velocity.x -= _velocity.x * _friction * dt;
	_velocity.z -= _velocity.z * _friction * dt;

	//�ʒu�X�V
	_gameobject->GetPosition() += _velocity * dt;
}

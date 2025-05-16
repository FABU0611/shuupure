//C_Move.h		移動コンポーネント
//20106_田中　蓮
//25_02_13
#pragma once
#include "Component.h"
#include "Input.h"

class Move : public Component {
	static const float	MOVE_NUM;
	XMFLOAT3			_velocity{};
	XMFLOAT3			_accel{ 25.0f, 25.0f, 25.0f };
	XMFLOAT3			_move{};
	float				_friction = 5.0f;

	BYTE				_forwardkey = 'W';
	BYTE				_backkey = 'S';
	BYTE				_rightkey = 'D';
	BYTE				_leftkey = 'A';

public:
	using Component::Component;

	void Update()override;

	void SetFowardKey(const BYTE& key) { _forwardkey = key; }
	void SetBackKey(const BYTE& key) { _backkey = key; }
	void SetRightKey(const BYTE& key) { _rightkey = key; }
	void SetLeftKey(const BYTE& key) { _leftkey = key; }

	const XMFLOAT3 GetMove()const { return _move; }
};
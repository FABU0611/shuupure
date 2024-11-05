//PlayerState_rotate.h
//20106_ìcíÜÅ@ò@
//24_08_28
#pragma once

#include "Main.h"
#include "PlayerState.h"
#include <fstream>
#include <string>

class PlayerStateRotate : public PlayerState {
	bool _isPosiRotateX = false;
	bool _isNegaRotateX = false;
	bool _isPosiRotateY = false;
	bool _isNegaRotateY = false;

	int _twistcount = 0;
	int _rotcount = 0;
	float	_dot = 0.0f;

	XMFLOAT3	_angulvel{};
	const float	_friction = 0.98f;

public:
	PlayerStateRotate(Player* player) :
		PlayerState(player) {}

	void Update()override;

	int GetTwistCount()const { return _twistcount; }
	int GetRotCount()const { return _rotcount; }
	float GetDot()const { return _dot; }
};


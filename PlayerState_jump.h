//PlayerState_jump.h
//20106_田中　蓮
//24_08_28
#pragma once

#include "PlayerState.h"

class PlayerStateJump : public PlayerState {
	float		_jump{};	//ジャンプ力用
	bool		_chage = false;

public:
	PlayerStateJump(Player* player) :
		PlayerState(player) {}

	void Update()override;
	bool GetChage()const { return _chage; }
};

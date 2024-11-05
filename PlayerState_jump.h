//PlayerState_jump.h
//20106_�c���@�@
//24_08_28
#pragma once

#include "PlayerState.h"

class PlayerStateJump : public PlayerState {
	float		_jump{};	//�W�����v�͗p
	bool		_chage = false;

public:
	PlayerStateJump(Player* player) :
		PlayerState(player) {}

	void Update()override;
	bool GetChage()const { return _chage; }
};

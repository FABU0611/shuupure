//PlayerState_end.h
//20106_“c’†@˜@
//24_08_28
#pragma once

#include "PlayerState.h"

class PlayerStateEnd : public PlayerState {
	float	_time;

public:
	PlayerStateEnd(Player* player) :
		PlayerState(player) {}

	void Update()override;
};


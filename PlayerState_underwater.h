//PlayerState_underwater.h
//20106_“c’†@˜@
//24_08_28
#pragma once

#include "PlayerState.h"

class PlayerStateUnderwater : public PlayerState {
	float				_time{};

public:
	PlayerStateUnderwater(Player* player):
	PlayerState(player){}

	void Update()override;
};
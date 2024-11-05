//PlayerState.h
//20106_“c’†@˜@
//24_08_28

#pragma once

class Player;

class PlayerState{
	Player*		_player;

protected:
	Player* GetPlayer() { return _player; }

public:
	PlayerState(Player* player) :
		_player(player) {}
	virtual ~PlayerState() {}

	virtual void Update() = 0;
};
//UIManager.h
//20106_ìcíÜÅ@ò@
//24_09_14
#pragma once

#include "DirectWriteCustomFont.h"
#include "GameObject.h"
#include "Player.h"
#include "PlayerState.h"

class UIManager : public GameObject{
	FontData*				_fontdata{};
	DirectWriteCustomFont*	_jump{};
	DirectWriteCustomFont*	_rotaion{};
	DirectWriteCustomFont*	_twist{};

	Player*					_player{};
	PlayerState*			_state{};

	float					_size{};
	float					_endsize{};
	float					_prog{};
	int						_rot{};
	int						_currot{};

public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
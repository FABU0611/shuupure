//CameraState_Game1.h
//20106_ìcíÜÅ@ò@
//24_12_12
#pragma once

#include "CameraState.h"

class GameState1 : public CameraState {
	static const float	CAMERA_SPEED;

public:
	GameState1(Camera* camera):CameraState(camera) {}
	~GameState1() {}

	void Update() override;
};
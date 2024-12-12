//CameraState_Title1.h
//20106_“c’†@˜@
//24_12_12
#pragma once

#include "CameraState.h"

class TitleState1 : public CameraState {

public:
	TitleState1(Camera* camera):CameraState(camera) {}
	~TitleState1() {}

	void Update() override;
};
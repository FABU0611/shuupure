//CameraState_Title2.h
//20106_“c’†@˜@
//24_12_12
#pragma once

#include "CameraState.h"

class TitleState2 : public CameraState {

public:
	TitleState2(Camera* camera):CameraState(camera) {}
	~TitleState2() {}

	void Update() override;
};
//S_Result.cpp	リザルトシーン
//20106_田中　蓮
//24_07_03
#pragma once

#include "Scene.h"

class Result : public Scene {
public:
	Result() {}
	~Result() {}

	void Init() override;
	void Update() override;
};
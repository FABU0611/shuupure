//S_Tutorial.h	チュートリアルシーン
//20106_田中　蓮
//24_09_17
#pragma once

#include "Scene.h"


class Tutorial : public Scene {
	LIGHT* _light = nullptr;

public:
	Tutorial() {}
	~Tutorial() {}

	void Init() override;
	void Uninit()override;
	void Update() override;
	void Draw()override;
};
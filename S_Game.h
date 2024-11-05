//S_Game.h	ゲームシーン
//20106_田中　蓮
//24_07_03
#pragma once

#include "Scene.h"


class Game : public Scene {
	LIGHT* _light = nullptr;

public:
	Game() {}
	~Game() {}

	void Init() override;
	void Uninit()override;
	void Update() override;
	void Draw()override;
};
//S_Title.h	タイトルシーン
//20106_田中　蓮
//24_07_03
#pragma once

#include "Scene.h"

class Title : public Scene {
	class Button* _start;

public:
	Title() {}
	~Title() {}

	void Init() override;
	void Update() override;
};
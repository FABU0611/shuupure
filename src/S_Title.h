//S_Title.h	�^�C�g���V�[��
//20106_�c���@�@
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
//S_Tutorial.h	�`���[�g���A���V�[��
//20106_�c���@�@
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
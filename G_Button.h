//G_Button.h
//20106_�c���@�@
//24_11_14
#pragma once

#include "GUI.h"

class Button : public GUI {
public:
	~Button() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
//G_Button.h
//20106_�c���@�@
//24_11_14
#pragma once
#include "GUI.h"

class Button : public GUI {
public:
	Button(const XMFLOAT3& pos, const XMFLOAT3& size);
	~Button() {}

	bool OnClicked(const int& mousebutton) override;
};
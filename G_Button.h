//G_Button.h
//20106_ìcíÜÅ@ò@
//24_11_14
#pragma once
#include "GUI.h"
#include <string>
#include "TextManager.h"

class Button : public GUI {
public:
	Button(const std::string& label, const D2D1::ColorF& color, const XMFLOAT3& pos, const XMFLOAT3& size);
	~Button() {}

	bool OnClicked(const MOUSE_BUTTON& mousebutton) override;
};
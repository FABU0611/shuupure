//G_ChackBox.h
//20106_�c���@�@
//24_12_04
#pragma once
#include "GUI.h"

class CheckBox : public GUI {
	bool	_ison{};

public:
	CheckBox(const XMFLOAT3& pos);
	~CheckBox() {}

	void Init()override;

	bool OnClicked(const int& mousebutton) override;
};
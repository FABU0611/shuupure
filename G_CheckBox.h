//G_ChackBox.h
//20106_�c���@�@
//24_12_04
#pragma once
#include "GUI.h"
#include "GUIManager.h"
#include <string>

class CheckBox : public GUI {
	bool	_ison{};

public:
	CheckBox(const std::string& label);
	~CheckBox() {}

	void Init()override;

	bool OnClicked(const MOUSE_BUTTON& mousebutton) override;
	XMFLOAT3 CalcPosition(){
		float pos = 200.0f + 75.0f * GUIManager::GetInstance()->GetCkeckBoxSize();
		return { 0.0f, pos, 0.0f };
	}
};
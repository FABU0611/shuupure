//G_ChackBox.h
//20106_ìcíÜÅ@ò@
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

	bool OnClicked(const int& mousebutton) override;
	XMFLOAT3 CalcPosition(){
		float pos = 100.0f * (GUIManager::GetInstance()->GetGUISize() + 2);
		return { 0.0f, pos, 0.0f };
	}
};
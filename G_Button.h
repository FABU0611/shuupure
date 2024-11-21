//G_Button.h
//20106_ìcíÜÅ@ò@
//24_11_14
#pragma once

#include "GUI.h"

class Button : public GUI {
public:
	Button(const XMFLOAT3& pos, const XMFLOAT3& size);
	~Button() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	bool IsHovered(const POINT pos) override;
	bool OnClicked(const int& mousebutton);
};
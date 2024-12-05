//G_Button.cpp
//20106_ìcíÜÅ@ò@
//24_11_14
#include "G_Button.h"
#include "Input.h"

Button::Button(const XMFLOAT3& pos, const XMFLOAT3& size, const wchar_t* texname)
	:GUI(pos, size, texname) {}

void Button::Init() {
	GUI::Init();
}

bool Button::OnClicked(const int& mousebutton) {
	if (IsHovered(Input::GetMousePosition())) {
		return Input::GetMouseTrigger(mousebutton);
	}
	return false;
}

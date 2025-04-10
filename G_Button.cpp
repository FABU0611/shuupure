//G_Button.cpp
//20106_“c’†@˜@
//24_11_14
#include "G_Button.h"
#include "Input.h"

Button::Button(const std::string& label, const D2D1::ColorF& color, const XMFLOAT3& pos, const XMFLOAT3& size)
	:GUI(pos, true, size, L"asset\\texture\\button.png") {
	auto text = TextManager::GetInstance()->AddText<Text>(size.y * 0.6f, color, TextAnchor::Center);
	text->SetString(label);
	text->SetPosition(pos);
}

bool Button::OnClicked(const int& mousebutton) {
	if (IsHovered(Input::GetMousePosition())) {
		return Input::GetMouseTrigger(mousebutton);
	}
	return false;
}

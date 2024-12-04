//G_ChackBox.cpp
//20106_田中　蓮
//24_12_04
#include "G_CheckBox.h"
#include "Renderer.h"

#include "C_Sprite2D.h"
#include "Input.h"

CheckBox::CheckBox(const XMFLOAT3& pos)
	:GUI(pos, XMFLOAT3(50.0f, 50.0f, 50.0f), L"asset\\texture\\checkbox.png") {}

void CheckBox::Init() {
	GUI::Init();

	_ison = false;
	GetComponent<Sprite2D>()->SetUV({ 0.5f, 0.0f });
	GetComponent<Sprite2D>()->SetUVend({ 0.5f, 1.0f });
}

bool CheckBox::OnClicked(const int& mousebutton) {
	if (IsHovered(Input::GetMousePosition())){
		Sprite2D* s = dynamic_cast<Sprite2D*>(GetComponent<Sprite2D>());
		if (!s) {
			return false;
		}
		if (Input::GetMouseTrigger(mousebutton)) {
			//クリックされたら色を暗く
			s->SetColor({ 0.5f, 0.5f, 0.5f, 1.0f });

			//切り替え
			_ison = !_ison;
			
			//状態によって見た目変更
			if (_ison) {
				s->SetUV({ 0.0f, 0.0f });
				s->SetUVend({ 0.5f, 1.0f });
			}
			else {
				s->SetUV({ 0.5f, 0.0f });
				s->SetUVend({ 0.5f, 1.0f });
			}

		}
		else if (Input::GetMouseRelease(mousebutton)) {
			s->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}
	return _ison;
}

//TutorialText.cpp
//20106_�c���@�@
//24_09_17
#include "T_Tutorial.h"

#include "Time.h"

void TutorialText::Init(){
	Text::Init();
	_spos.x = _epos.x = GetPosition().x = 90.0f;
	_spos.y = _epos.y = 450.0f;

	_str = "��]\tW,S Key\n"
		"�P��\tA,D Key\n\n"
		"�J������]\tI,J,K,L Key\n\n"
		"�Q�[���J�n\tSpace Key";
}

void TutorialText::Update(){
	//�㉺
	_spos.y += Time::GetDeltaTime();
	GetPosition().y = cos(_spos.y * 2) + _epos.y;

	_text->SetFont(_fontdata);
}
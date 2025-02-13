//TutorialText.cpp
//20106_田中　蓮
//24_09_17
#include "T_Tutorial.h"

#include "Time.h"

void TutorialText::Init(){
	Text::Init();
	_spos.x = _epos.x = GetPosition().x = 90.0f;
	_spos.y = _epos.y = 450.0f;

	_str = "回転\tW,S Key\n"
		"捻り\tA,D Key\n\n"
		"カメラ回転\tI,J,K,L Key\n\n"
		"ゲーム開始\tSpace Key";
}

void TutorialText::Update(){
	//上下
	_spos.y += Time::GetDeltaTime();
	GetPosition().y = cos(_spos.y * 2) + _epos.y;

	_text->SetFont(_fontdata);
}
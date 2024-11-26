//Title.cpp
//20106_ìcíÜÅ@ò@
//24_09_15
#include "T_Title.h"

#include "Time.h"

void TitleText::Init(){
	Text::Init();

	_spos.x = _epos.x = GetPosition().x = (SCREEN_WIDTH * 0.5f) - 330.0f;
	_spos.y = 100.0f;
	_epos.y = 100.0f;

	_str = "Spin Dive";
}

void TitleText::Update(){
	//è„â∫
	_spos.y += Time::GetDeltaTime();
	GetPosition().y = cos(_spos.y * 2) + _epos.y;
}

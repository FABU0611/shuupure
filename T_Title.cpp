//Title.cpp
//20106_�c���@�@
//24_09_15
#include "T_Title.h"

#include "Time.h"

void TitleText::Init(){
	Text::Init();

	_spos.x = _epos.x = GetPosition().x = (SCREEN_WIDTH * 0.5f);
	_spos.y = 100.0f;
	_epos.y = 100.0f;

	_str = "�V�F�[�_�[�R����t�H�g���[�h";
}

void TitleText::Update(){
	//�㉺
	_spos.y += Time::GetDeltaTime();
	GetPosition().y = cos(_spos.y * 2) + _epos.y;
}

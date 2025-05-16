//TutorialText.cpp
//20106_“c’†@˜@
//24_09_17
#include "T_Tutorial.h"

#include "Time.h"

void TutorialText::Init(){
	Text::Init();
	_spos.x = _epos.x = GetPosition().x = 90.0f;
	_spos.y = _epos.y = 450.0f;

	_str = "‰ñ“]\tW,S Key\n"
		"”P‚è\tA,D Key\n\n"
		"ƒJƒƒ‰‰ñ“]\tI,J,K,L Key\n\n"
		"ƒQ[ƒ€ŠJŽn\tSpace Key";
}

void TutorialText::Update(){
	//ã‰º
	_spos.y += Time::GetDeltaTime();
	GetPosition().y = cos(_spos.y * 2) + _epos.y;

	_text->SetFont(_fontdata);
}
//TutorialText.h
//20106_�c���@�@
//24_09_17
#pragma once
#include "Text.h"

class TutorialText : public Text {
	XMFLOAT2				_spos{};
	XMFLOAT2				_epos{};

public:
	TutorialText() {}
	TutorialText(const float& size, const D2D1::ColorF& color)
	:Text(size, color){}
	~TutorialText() {}

	void Init()override;
	void Update()override;
};
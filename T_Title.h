//Title.h
//20106_ìcíÜÅ@ò@
//24_09_15
#pragma once

#include "Text.h"

class TitleText : public Text {
	XMFLOAT2				_spos{};
	XMFLOAT2				_epos{};

public:
	TitleText() {}
	TitleText(const float& size, const D2D1::ColorF& color)
		:Text(size, color) {}
	~TitleText() {}

	void Init()override;
	void Update()override;
};
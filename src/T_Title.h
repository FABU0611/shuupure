//Title.h
//20106_�c���@�@
//24_09_15
#pragma once

#include "Text.h"

class TitleText : public Text {
	XMFLOAT2				_spos{};
	XMFLOAT2				_epos{};

public:
	TitleText() {}
	TitleText(const float& size, const D2D1::ColorF& color, const TextAnchor& anchor)
		:Text(size, color, anchor) {}
	~TitleText() {}

	void Init()override;
	void Update()override;
};
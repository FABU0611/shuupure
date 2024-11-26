//T_Loading.h
//20106_ìcíÜÅ@ò@
//24_11_21
#pragma once

#include "Text.h"

class LoadingText : public Text {
	float					_time{};
	float					_fadealpha{};

public:
	LoadingText() {}
	LoadingText(const float& size, const D2D1::ColorF& color)
		:Text(size, color) {}
	~LoadingText() {}

	void Init()override;
	void Update()override;

	void SetFadealpha(const float& alpha) { _fadealpha = alpha; }
};
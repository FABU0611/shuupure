//TitlePush.h
//20106_�c���@�@
//24_09_15
#pragma once
#include "Text.h"


class TitlePush : public Text {
	int						_start{};

public:
	TitlePush() {}
	TitlePush(const float& size, const D2D1::ColorF& color, const TextAnchor& anchor)
		:Text(size, color, anchor) {}
	~TitlePush() {}

	void Init()override;
	void Update()override;
};
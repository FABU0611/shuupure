//TitlePush.h
//20106_ìcíÜÅ@ò@
//24_09_15
#pragma once
#include "Text.h"


class TitlePush : public Text {
	XMFLOAT2				_spos{};
	XMFLOAT2				_epos{};
	float					_time{};

	int						_start{};

public:
	TitlePush() {}
	TitlePush(const float& size, const D2D1::ColorF& color, const TextAnchor& anchor)
		:Text(size, color, anchor) {}
	~TitlePush() {}

	void Init()override;
	void Update()override;
};
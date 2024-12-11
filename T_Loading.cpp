//T_Loading.cpp
//20106_ìcíÜÅ@ò@
//24_11_21
#include "T_Loading.h"

const float LoadingText::STEP_TIME = 0.2f;

void LoadingText::Init() {
	Text::Init();

	_time = 0.0f;
	GetPosition() = { 10.0f, 10.0f, 0.0f };
}

void LoadingText::Update() {
	_time += 0.1f;

	int idx = (int)(_time / STEP_TIME);
	if (idx < 8) {
		_str = _strings[idx];
	}
	else {
		_time = 0.0f;
		_str = _strings[0];
	}

	SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, _fadealpha));
}
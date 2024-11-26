//T_Loading.cpp
//20106_“c’†@˜@
//24_11_21
#include "T_Loading.h"

void LoadingText::Init() {
	Text::Init();

	_time = 0.0f;
	GetPosition() = { 10.0f, 10.0f, 0.0f };
}

void LoadingText::Update() {
	_time += 0.1f;
	if (_time >= 0.2f) {
		_str = "L";
	}
	if (_time >= 0.4f) {
		_str = "Lo";
	}
	if (_time >= 0.6f) {
		_str = "Loa";
	}
	if (_time >= 0.8f) {
		_str = "Load";
	}
	if (_time >= 1.0f) {
		_str = "Loadi";
	}
	if (_time >= 1.2f) {
		_str = "Loadin";
	}
	if (_time >= 1.4f) {
		_str = "Loading";
	}
	if (_time >= 1.6f) {
		_time = 0.0f;
		_str = "";
	}
	SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, _fadealpha));
}
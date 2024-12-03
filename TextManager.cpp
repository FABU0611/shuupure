//TextManager.cpp
//20106_“c’†@˜@
//24_11_21
#include "TextManager.h"


void TextManager::Uninit() {
	for (auto& t : _texts) {
		if (!t) {
			continue;
		}
		t->Uninit();
		delete t;
		t = nullptr;
	}
	_texts.clear();
}

void TextManager::Update() {
	for (auto& t : _texts) {
		if (!t) {
			continue;
		}
		t->Update();
	}
}

void TextManager::Draw() {
	for (auto& t : _texts) {
		if (!t) {
			continue;
		}
		t->Draw();
	}
}

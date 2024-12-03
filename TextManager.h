//TextManager.h
//20106_ìcíÜÅ@ò@
//24_11_21
#pragma once

#include <list>
#include "Text.h"

class TextManager {
	std::list<Text*>	_texts;

public:
	TextManager() {}
	~TextManager() {
		Uninit();
	}

	void Uninit();
	void Update();
	void Draw();

	template <typename T, typename... Args>
	void AddText(Args&& ...args) {
		T* text = new T(std::forward<Args>(args)...);
		_texts.push_back(text);
		text->Init();
	}
};
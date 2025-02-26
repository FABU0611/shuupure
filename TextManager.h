//TextManager.h
//20106_田中　蓮
//24_11_21
#pragma once

#include <list>
#include "Text.h"

//テキストを管理するクラス
class TextManager {
	TextManager() = default;
	~TextManager() = default;
	TextManager(const TextManager&) = delete;
	TextManager& operator=(const TextManager&) = delete;

	std::list<Text*>	_texts;

	static TextManager* _instance;

public:
	void Uninit();
	void Update();
	void Draw();

	static TextManager* GetInstance() {
		if (!_instance) {
			_instance = new TextManager();
		}
		return _instance;
	}
	static void DeleteInstance() {
		if (_instance) {
			_instance->Uninit();
			delete _instance;
			_instance = nullptr;
		}
	}

	template <typename T, typename... Args>
	void AddText(Args&& ...args) {
		T* text = new T(std::forward<Args>(args)...);
		_texts.push_back(text);
		text->Init();
	}
};
//GUIManager.h
//20106_“c’†@˜@
//24_11_17
#pragma once
#include <list>
#include "GameObject.h"

class GUIManager {
	std::list<GameObject*>	_guis;

public:
	GUIManager() {}
	~GUIManager() {
		for (auto g : _guis) {
			if (!g) {
				continue;
			}
			delete g;
		}
		_guis.clear();
	}

	void Draw() {
		for (auto g : _guis) {
			g->Draw();
		}
	}
};
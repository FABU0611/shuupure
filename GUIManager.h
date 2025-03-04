//GUIManager.h
//20106_田中　蓮
//24_11_17
#pragma once
#include <vector>
#include "GUI.h"
#include "Manager.h"
#include "Scene.h"

class GUIManager {
	std::vector<GUI*>	_guis;		//要素番号が欲しいのでvector

public:
	GUIManager() {}
	~GUIManager() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	template <typename T, typename... Args>
	int AddGUI(Args&& ...args) {
		GUI* gui = new T(std::forward<Args>(args)...);
		_guis.push_back(gui);
		gui->Init();

		return (int)_guis.size() - 1;
	}
	GUI* GetGUI(const int& index);
	void Clear();
};
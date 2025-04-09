//GUIManager.h
//20106_ìcíÜÅ@ò@
//24_11_17
#pragma once
#include <vector>
#include "GUI.h"


class GUIManager {
	GUIManager() = default;
	~GUIManager() = default;
	GUIManager(const GUIManager&) = delete;
	GUIManager& operator=(const GUIManager&) = delete;
	
	static void DeleteInstance();
	std::vector<GUI*>	_guis;		//óvëfî‘çÜÇ™ó~ÇµÇ¢ÇÃÇ≈vector

	static GUIManager* _instance;
	void Uninit();

	friend class Manager;

public:
	static GUIManager* GetInstance();

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
	int GetGUISize() { return (int)_guis.size(); }
};
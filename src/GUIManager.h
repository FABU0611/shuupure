//GUIManager.h
//20106_ìcíÜÅ@ò@
//24_11_17
#pragma once
#include <list>
#include "GUI.h"


class GUIManager {
	GUIManager() = default;
	~GUIManager() = default;
	GUIManager(const GUIManager&) = delete;
	GUIManager& operator=(const GUIManager&) = delete;
	
	static void DeleteInstance();
	std::list<GUI*>	_guis;

	static GUIManager* _instance;
	void Uninit();

	friend class Manager;

public:
	static GUIManager* GetInstance();

	void Update();
	void Draw();

	template <typename T, typename... Args>
	T* AddGUI(Args&& ...args) {
		T* gui = new T(std::forward<Args>(args)...);
		_guis.push_back(gui);
		gui->Init();

		return gui;
	}
	int GetGUISize() { return (int)_guis.size(); }
	int GetCkeckBoxSize();
};
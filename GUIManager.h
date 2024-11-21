//GUIManager.h
//20106_�c���@�@
//24_11_17
#pragma once
#include <vector>
#include "GUI.h"
#include "Manager.h"
#include "Scene.h"

class GUIManager {
	static std::vector<GUI*>	_guis;

public:
	GUIManager() {}
	~GUIManager() {}

	template <typename T, typename... Args>
	static int AddGUI(Args&& ...args) {
		//���݂̃V�[����GUI��ǉ�
		Scene* scene = Manager::GetScene();
		GUI* gui = scene->AddGameobject<T>(std::forward<Args>(args)...);

		//GUIManager�ɓo�^
		_guis.push_back(gui);
		return _guis.size() - 1;
	}
	static GUI* GetGUI(const int& index);
	static void Clear();
};
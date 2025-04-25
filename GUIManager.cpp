//GUIManager.cpp
//20106_“c’†@˜@
//24_11_17
#include "GUIManager.h"
#include "Manager.h"
#include "Scene.h"
#include "G_CheckBox.h"

GUIManager* GUIManager::_instance = nullptr;

GUIManager* GUIManager::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GUIManager();
	}
	return _instance;
}

void GUIManager::DeleteInstance() {
	if (!_instance) {
		return;
	}
	_instance->Uninit();
	delete _instance;
	_instance = nullptr;
}

void GUIManager::Uninit() {
	for (auto& g : _instance->_guis) {
		if (!g) {
			continue;
		}
		g->Uninit();
		delete g;
		g = nullptr;
	}
	_instance->_guis.clear();
}

void GUIManager::Update() {
	for (auto& g : _instance->_guis) {
		g->Update();
	}
}

void GUIManager::Draw() {
	for (auto& g : _instance->_guis) {
		g->Draw();
	}
}

int GUIManager::GetCkeckBoxSize() {
	int count = 0;
	for (auto& g : _instance->_guis) {
		if (!dynamic_cast<CheckBox*>(g)) {
			continue;
		}
		count++;
	}
	return count;
}

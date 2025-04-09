//GUIManager.cpp
//20106_“c’†@˜@
//24_11_17
#include "GUIManager.h"
#include "Manager.h"
#include "Scene.h"

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
	for (auto& g : _guis) {
		if (!g) {
			continue;
		}
		g->Uninit();
		delete g;
		g = nullptr;
	}
	_guis.clear();
}

void GUIManager::Update() {
	for (auto& g : _guis) {
		g->Update();
	}
}

void GUIManager::Draw() {
	for (auto& g : _guis) {
		g->Draw();
	}
}

GUI* GUIManager::GetGUI(const int& index) {
	if (index >= 0 && index < _guis.size()) {
		return _guis[index];
	}
	return nullptr;
}
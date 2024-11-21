//GUIManager.cpp
//20106_“c’†@˜@
//24_11_17
#include "GUIManager.h"

std::vector<GUI*> GUIManager::_guis;


GUI* GUIManager::GetGUI(const int& index) {
	if (index >= 0 && index < _guis.size()) {
		return _guis[index];
	}
	return nullptr;
}

void GUIManager::Clear() {
	_guis.clear();
}

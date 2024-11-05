//Component.h
//20106_ìcíÜÅ@ò@
//24_05_22
#pragma once

class Component {
protected:
	class GameObject* _gameobject = nullptr;

public:
	Component() = delete;
	Component(GameObject* object) { _gameobject = object; }
	virtual ~Component() {}

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
};
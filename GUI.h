//GUI.h
//20106_ìcíÜÅ@ò@
//24_11_14
#pragma once

#include "GameObject.h"

class GUI : public GameObject{
public:
	GUI() = delete;
	virtual ~GUI() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
//LightCamera.h
//20106_ìcíÜÅ@ò@
//25_01_13
#pragma once
#include "GameObject.h"
#include "Renderer.h"


class LightCamera : public GameObject {
	LIGHT		_light{};
	XMFLOAT3	_target{};

public:
	LightCamera() {}
	~LightCamera() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
//CheckCameraDepth.h
//20106_�c���@�@
//25_01_17
#pragma once
#include "Main.h"


class CheckCameraDepth {
	ID3D11Buffer* _vertexbuffer = NULL;

	int					_screen{};

public:
	CheckCameraDepth() {}
	~CheckCameraDepth() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
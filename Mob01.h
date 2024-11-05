//Mob01.h
//20106_ìcíÜÅ@ò@
//24_09_17
#pragma once

#include "GameObject.h"

class Mob01 : public GameObject {
	ID3D11VertexShader*	_vertexShader;
	ID3D11PixelShader*	_pixelShader;
	ID3D11InputLayout*	_vertexLayout;

	int	_animationFrame;

public:
	Mob01(const XMFLOAT3& pos, const XMFLOAT3& scl);
	~Mob01() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Hit(GameObject* obj) {}
};
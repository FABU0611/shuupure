//field.h
//20106_ìcíÜÅ@ò@
//2024_05_08
#pragma once
#include "GameObject.h"
#include "C_Sprite3D.h"
#include "Renderer.h"

class Water : public GameObject{
	XMFLOAT4					_param{};
	XMFLOAT3					_size{};

public:
	Water(const XMFLOAT3& size):
	_size(size){
		SetScale({ size.x, size.y, size.z });
		AddComponent<Sprite3D>(this);
		GetComponent<Sprite3D>()->LoadTexture(L"asset\\texture\\fade.png");
		GetComponent<Sprite3D>()->SetTangent({ 1.0f, 0.0f, 0.0f });
		GetComponent<Sprite3D>()->SetNormal({ 0.0f, 0.0f, -1.0f });
		GetComponent<Sprite3D>()->LoadNormalTexture(L"asset\\texture\\water_normal.dds");
		GetComponent<Sprite3D>()->LoadEnvTexture(L"asset\\model\\sky02.jpg");
		GetComponent<Sprite3D>()->SetDrawMode(DrawMode3D::Normal);
	}
	~Water() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
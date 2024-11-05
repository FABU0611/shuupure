//Fade.h
//20106_ìcíÜÅ@ò@
//24_09_17
#pragma once

#include "GameObject.h"
#include "C_Sprite2D.h"

class Fade : public GameObject {
	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;

public:
	Fade() {
		AddComponent<Sprite2D>(this);
		GetComponent<Sprite2D>()->SetDrawMode(DrawMode::LeftTopColor);
		GetComponent<Sprite2D>()->SetSize({ SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f });
	}
	~Fade() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetColor(const XMFLOAT4& color) {
		GetComponent<Sprite2D>()->SetColor(color);
	}
};
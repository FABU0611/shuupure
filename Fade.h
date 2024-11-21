//Fade.h
//20106_ìcíÜÅ@ò@
//24_09_17
#pragma once

#include "GameObject.h"
#include "C_Sprite2D.h"

enum class FadeMode {
	None,
	In,
	Out,
};

class Fade : public GameObject {
	ID3D11VertexShader* _vertexshader{};
	ID3D11PixelShader*	_pixelshader{};
	ID3D11InputLayout*	_vertexlayout{};

	XMFLOAT4			_fadecolor{};
	float				_faderate{};

	FadeMode			_mode = FadeMode::None;

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

	void LoadTexture() { GetComponent<Sprite2D>()->LoadTexture(L"asset\\texture\\fade.png"); }

	void SetColor() { GetComponent<Sprite2D>()->SetColor(_fadecolor); }
	void SetFaderate(const float& rate) { _faderate = rate; }

	XMFLOAT4& GetFadeColor() { return _fadecolor; }
	float GetFaderate()const { return _faderate; }
	FadeMode& GetFadeMode() { return _mode; }
};
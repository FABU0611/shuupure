//GUI.cpp
//20106_田中　蓮
//24_12_04
#include "GUI.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "C_Sprite2D.h"
#include "Input.h"

GUI::GUI(const XMFLOAT3& pos, const XMFLOAT3& size, const wchar_t* texname) {
	_texname = texname;
	SetPosition(pos);
	_size = size;
}

void GUI::Init() {
	AddComponent<Sprite2D>(this);
	Sprite2D* sprite2d = GetComponent<Sprite2D>();
	sprite2d->LoadTexture(_texname);
	sprite2d->SetDrawMode(DrawMode::Color);
	sprite2d->SetSize(_size);
}

void GUI::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}
}

void GUI::Draw() {
	Shader::SetShader(ShaderName::Particle);

	GetComponent<Sprite2D>()->SetPos(GetPosition());
	for (auto c : _components) {
		c->Draw();
	}
}

bool GUI::IsHovered(const POINT pos) {
	XMFLOAT2 position = { GetPosition().x, GetPosition().y };
	XMFLOAT2 size = { GetComponent<Sprite2D>()->GetSize().x, GetComponent<Sprite2D>()->GetSize().y };

	//マウスが重なったら色を少し暗く
	if (position.x - size.x * 0.5f < pos.x && position.x + size.x * 0.5f > pos.x &&
		position.y - size.y * 0.5f < pos.y && position.y + size.y * 0.5f > pos.y) {
		GetComponent<Sprite2D>()->SetColor({ 0.75f, 0.75f, 0.75f, 1.0f });
		return true;
	}
	else {
		GetComponent<Sprite2D>()->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	return false;
}

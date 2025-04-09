//GUI.cpp
//20106_田中　蓮
//24_12_04
#include "GUI.h"
#include "Renderer.h"
#include "C_Sprite2D.h"
#include "ShaderManager.h"
#include "Input.h"

GUI::GUI(const XMFLOAT3& pos, const bool& iscenter, const XMFLOAT3& size, const wchar_t* texname) {
	_texname = texname;
	_iscenter = iscenter;

	SetPosition(pos);
	_size = size;
	AddComponent<Sprite2D>(this);
	Sprite2D* sprite2d = GetComponent<Sprite2D>();
	sprite2d->LoadTexture(_texname);
	if (iscenter) {
		sprite2d->SetDrawMode(DrawMode::Color);
	}
	else {
		sprite2d->SetDrawMode(DrawMode::LeftTopColor);
	}
	sprite2d->SetSize(_size);
}

void GUI::Init() {
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

	bool isHovered = false;

	if (_iscenter) {
		isHovered = (position.x - size.x * 0.5f < pos.x && position.x + size.x * 0.5f > pos.x && position.y - size.y * 0.5f < pos.y && position.y + size.y * 0.5f > pos.y);
	}
	else {
		isHovered = (position.x < pos.x && position.x + size.x > pos.x && position.y < pos.y && position.y + size.y > pos.y);
	}

	//マウスが重なったら色を少し暗く
	if (isHovered) {
		GetComponent<Sprite2D>()->SetColor({ 0.75f, 0.75f, 0.75f, 1.0f });
		return true;
	}
	else {
		GetComponent<Sprite2D>()->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	return false;
}

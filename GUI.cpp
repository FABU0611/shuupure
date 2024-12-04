//GUI.cpp
//20106_田中　蓮
//24_12_04
#include "GUI.h"
#include "Renderer.h"

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

	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\UnlitTextureVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\ParticlePS.cso");
}

void GUI::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

void GUI::Draw() {
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

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

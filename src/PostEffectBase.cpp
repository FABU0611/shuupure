//PostEffectBase.h
//20106_ìcíÜÅ@ò@
//25_05_24
#include "PostEffectBase.h"
#include "Renderer.h"
#include "C_Sprite2D.h"

void PostEffectBase::Init() {
	Sprite2D* sprite = AddComponent<Sprite2D>(this);
	sprite->CreateVertexBuffer(1);
	sprite->SetDrawMode(DrawMode::PostEffect);
	sprite->SetSize({ SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f });
}

void PostEffectBase::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}
}

void PostEffectBase::Draw() {
	for (auto c : _components) {
		c->Draw();
	}
}

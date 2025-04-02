//sky.cpp
//20106_田中　蓮
//24_06_05
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "ShaderManager.h"
#include "Sky.h"
#include "C_Transform.h"
#include "C_ModelRenderer.h"

void Sky::Init() {
	AddComponent<ModelRenderer>(this);

	//モデル読み込み
	GetComponent<ModelRenderer>()->Load("asset\\model\\sky.obj");

	//GetRotation().y = XM_PI * 0.5f;
}

void Sky::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}
}

void Sky::Update() {
}

void Sky::Draw() {
	Shader::SetShader(ShaderName::Unlit);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world, _prevworld);

	for (auto c : _components) {
		c->Draw();
	}
}

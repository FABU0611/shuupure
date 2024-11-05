//sky.cpp
//20106_田中　蓮
//24_06_05
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Sky.h"
#include "C_Transform.h"
#include "C_AnimationModel.h"

void Sky::Init() {
	AddComponent<AnimationModel>(this);

	//モデル読み込み
	GetComponent<AnimationModel>()->Load("asset\\model\\skydoom.fbx");


	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Sky::Uninit() {
	_VertexShader->Release();
	_PixelShader->Release();
	_VertexLayout->Release();
}

void Sky::Update() {
}

void Sky::Draw() {
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	for (auto c : _components) {
		c->Draw();
	}
}

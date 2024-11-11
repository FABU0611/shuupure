//Skyobj.cpp
//20106_田中　蓮
//24_09_17
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Skyobj.h"
#include "C_Transform.h"
#include "C_ModelRenderer.h"

void Skyobj::Init() {
	AddComponent<ModelRenderer>(this);

	//モデル読み込み
	GetComponent<ModelRenderer>()->Load("asset\\model\\sky.obj");


	Renderer::CreateVertexShader(&_vertexshader, &_vertexLayout,
		"shader\\UnlitTextureVS.cso");

	Renderer::CreatePixelShader(&_pixelShader,
		"shader\\UnlitTexturePS.cso");
}

void Skyobj::Uninit() {
	_vertexshader->Release();
	_pixelShader->Release();
	_vertexLayout->Release();
}

void Skyobj::Update() {
}

void Skyobj::Draw() {
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelShader, NULL, 0);

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

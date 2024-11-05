//DivingBoard.h	飛び込み台
//20106_田中　蓮
//24_09_07

#include "Pool.h"
#include "Renderer.h"
#include "C_AnimationModel.h"

void Pool::Init(){
	AddComponent<AnimationModel>(this);

	//モデル読み込み
	GetComponent<AnimationModel>()->Load("asset\\model\\pool.fbx");

	//シェーダーセット
	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\TangentNormalLightingVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\TangentNormalLightingPS.cso");
}

void Pool::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}

	_VertexShader->Release();
	_PixelShader->Release();
	_VertexLayout->Release();
}

void Pool::Update(){
	for (auto c : _components) {
		c->Update();
	}
}

void Pool::Draw(){
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

//DivingBoard.h	飛び込み台
//20106_田中　蓮
//24_09_07

#include "DivingBoard.h"
#include "Renderer.h"
#include "C_AnimationModel.h"

void DivingBoard::Init(){
	AddComponent<AnimationModel>(this);

	//モデル読み込み
	GetComponent<AnimationModel>()->Load("asset\\model\\jumpdai.fbx");

	//シェーダーセット
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\DepthShadowVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\DepthShadowPS.cso");
}

void DivingBoard::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

void DivingBoard::Update(){
	for (auto c : _components) {
		c->Update();
	}
}

void DivingBoard::Draw(){
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

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

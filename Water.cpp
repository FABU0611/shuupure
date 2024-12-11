//field.cpp
//20106_田中　蓮
//24_05_08
#include "Main.h"
#include "Renderer.h"
#include "Water.h"

//初期化処理
void Water::Init(){
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\WaterSurfaceVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\WaterSurfacePS.cso");

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\model\\sky02.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_envtexture);
	assert(_envtexture);

	SetRotation({ 90.0f, 0.0f, 0.0f });
	SetPosition({ 15.0f, -0.05f, 0.0 });

	GetComponent<Sprite3D>()->SetUVend({25.0f, 25.0f});
}

//終了処理
void Water::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

//更新処理
void Water::Update() {
}

//描画処理
void Water::Draw(){
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &_envtexture);

	//Zバッファ無効
	Renderer::SetDepthEnable(false);	//パーティクルをZソートするのは負荷が高い
	for (auto c : _components) {
		c->Draw();
	}
	//Zバッファ無効
	Renderer::SetDepthEnable(true);	//パーティクルをZソートするのは負荷が高い
}

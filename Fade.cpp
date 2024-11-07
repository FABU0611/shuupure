//Fade.cpp
//20106_田中　蓮
//24_09_17
#include "Fade.h"
#include "Renderer.h"

void Fade::Init(){
	GetComponent<Sprite2D>()->LoadTexture(L"asset\\texture\\fade.png");

	//シェーダーセット
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\unlitTexturePS.cso");
}

void Fade::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}
	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

void Fade::Update(){
}

void Fade::Draw(){
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

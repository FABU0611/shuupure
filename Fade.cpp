//Fade.cpp
//20106_�c���@�@
//24_09_17
#include "Fade.h"
#include "Renderer.h"

void Fade::Init(){
	GetComponent<Sprite2D>()->LoadTexture(L"asset\\texture\\fade.png");

	//�V�F�[�_�[�Z�b�g
	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void Fade::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}
	_VertexShader->Release();
	_PixelShader->Release();
	_VertexLayout->Release();
}

void Fade::Update(){
}

void Fade::Draw(){
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	GetComponent<Sprite2D>()->SetPos(GetPosition());

	for (auto c : _components) {
		c->Draw();
	}
}

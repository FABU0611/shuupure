//Fade.cpp
//20106_�c���@�@
//24_09_17
#include "Fade.h"
#include "Renderer.h"

void Fade::Init(){
	GetComponent<Sprite2D>()->LoadTexture(L"asset\\texture\\fade.png");

	//�V�F�[�_�[�Z�b�g
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\UnlitTextureVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\UnlitTexturePS.cso");
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
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

	GetComponent<Sprite2D>()->SetPos(GetPosition());

	for (auto c : _components) {
		c->Draw();
	}
}

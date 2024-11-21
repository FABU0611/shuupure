//Fade.cpp
//20106_�c���@�@
//24_09_17
#include "Fade.h"
#include "Renderer.h"
#include "Manager.h"

void Fade::Init(){
	LoadTexture();

	//�V�F�[�_�[�Z�b�g
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\UnlitTextureVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\UnlitTexturePS.cso");

	_fadecolor = { 1.0f, 1.0f, 1.0f, 0.0f };
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
	if (_mode == FadeMode::None) {
		return;
	}
	//�t�F�[�h�A�E�g����
	if (_mode == FadeMode::Out) {
		_fadecolor.w += _faderate;

		if (_fadecolor.w >= 1.0f) {
			//�t�F�[�h�C�������ɐ؂�ւ�
			_fadecolor.w = 1.0f;
			_mode = FadeMode::In;

			//���[�h��ݒ�
			Manager::ChangeNextScene();
		}
	}
	//�t�F�[�h�C������
	else if (_mode == FadeMode::In) {
		_fadecolor.w -= _faderate;

		if (_fadecolor.w <= 0.0f) {
			//�t�F�[�h�����I��
			_fadecolor.w = 0.0f;
			_mode = FadeMode::None;
		}
	}
	//�X�v���C�g�̐F�X�V
	SetColor();
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

//field.cpp
//20106_�c���@�@
//24_05_08
#include "Main.h"
#include "Renderer.h"
#include "Water.h"

//����������
void Water::Init(){
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\WaterSurfaceVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\WaterSurfacePS.cso");

	//�e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\model\\sky02.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_envtexture);
	assert(_envtexture);

	SetRotation({ 90.0f, 0.0f, 0.0f });
	SetPosition({ 15.0f, -0.05f, 0.0 });

	GetComponent<Sprite3D>()->SetUVend({25.0f, 25.0f});
}

//�I������
void Water::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

//�X�V����
void Water::Update() {
}

//�`�揈��
void Water::Draw(){
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &_envtexture);

	//Z�o�b�t�@����
	Renderer::SetDepthEnable(false);	//�p�[�e�B�N����Z�\�[�g����͕̂��ׂ�����
	for (auto c : _components) {
		c->Draw();
	}
	//Z�o�b�t�@����
	Renderer::SetDepthEnable(true);	//�p�[�e�B�N����Z�\�[�g����͕̂��ׂ�����
}

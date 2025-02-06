//field.cpp
//20106_�c���@�@
//24_05_08
#include "Main.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Time.h"
#include "Water.h"

//����������
void Water::Init(){
	//�e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\model\\sky02.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_envtexture);
	assert(_envtexture);

	SetRotation({ 90.0f, 0.0f, 0.0f });
	SetPosition({ 15.0f, -0.05f, 0.0 });

	Sprite3D* sprite = GetComponent<Sprite3D>();
	sprite->SetUVend({ 25.0f, 25.0f });
	sprite->SetColor({ 0.2f, 0.2f, 1.0f, 1.0f });

	_param = { 0.0f, 0.0f, 1.0f, 0.0f };
}

//�I������
void Water::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}
}

//�X�V����
void Water::Update() {
	_param.x += 0.5f * Time::GetDeltaTime();
	_param.y += 0.5f * Time::GetDeltaTime();
	if (_param.x >= 1.0f) {
		_param.x = 0.0f;
	}
	if (_param.y >= 1.0f) {
		_param.y = 0.0f;
	}
}

//�`�揈��
void Water::Draw(){
	Shader::SetShader(ShaderName::Water);

	Renderer::SetParameter(_param);

	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &_envtexture);

	for (auto c : _components) {
		c->Draw();
	}
}

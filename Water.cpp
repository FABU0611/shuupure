//field.cpp
//20106_�c���@�@
//24_05_08
#include "Main.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Time.h"
#include "Water.h"

//����������
void Water::Init(){
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

	for (auto c : _components) {
		c->Draw();
	}
}

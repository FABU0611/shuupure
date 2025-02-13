//field.cpp
//20106_田中　蓮
//24_05_08
#include "Main.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "ErrorHandler.h"
#include "Time.h"
#include "Water.h"

//初期化処理
void Water::Init(){
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	ErrorHandler::GetInstance()->LoadTex(L"asset\\model\\sky02.jpg", metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_envtexture);
	assert(_envtexture);
	//_envtexture = TextureManager::LoadTexture(L"asset\\model\\sky02.jpg");

	SetRotation({ 90.0f, 0.0f, 0.0f });
	SetPosition({ 15.0f, -0.05f, 0.0 });

	Sprite3D* sprite = GetComponent<Sprite3D>();
	sprite->SetUVend({ 25.0f, 25.0f });
	sprite->SetColor({ 0.2f, 0.2f, 1.0f, 1.0f });

	_param = { 0.0f, 0.0f, 1.0f, 0.0f };
}

//終了処理
void Water::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}
}

//更新処理
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

//描画処理
void Water::Draw(){
	Shader::SetShader(ShaderName::Water);

	Renderer::SetParameter(_param);

	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &_envtexture);

	for (auto c : _components) {
		c->Draw();
	}
}

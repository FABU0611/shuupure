//Fade.cpp
//20106_田中　蓮
//24_09_17
#include "Fade.h"
#include "Renderer.h"
#include "Manager.h"
#include "ShaderManager.h"

void Fade::Init(){
	LoadTexture();

	_fadecolor = { 1.0f, 1.0f, 1.0f, 0.0f };
}

void Fade::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}
}

void Fade::Update(){
	if (_mode == FadeMode::None) {
		return;
	}
	//フェードアウト処理
	if (_mode == FadeMode::Out) {
		_fadecolor.w += _faderate;

		if (_fadecolor.w >= 1.0f) {
			//フェードイン処理に切り替え
			_fadecolor.w = 1.0f;
			_mode = FadeMode::In;

			//モードを設定
			Manager::ChangeNextScene();
		}
	}
	//フェードイン処理
	else if (_mode == FadeMode::In) {
		_fadecolor.w -= _faderate;

		if (_fadecolor.w <= 0.0f) {
			//フェード処理終了
			_fadecolor.w = 0.0f;
			_mode = FadeMode::None;
		}
	}
	//スプライトの色更新
	SetColor();
}

void Fade::Draw(){
	Shader::SetShader(ShaderName::Unlit);

	GetComponent<Sprite2D>()->SetPos(GetPosition());

	for (auto c : _components) {
		c->Draw();
	}
}

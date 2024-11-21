#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "C_ModelRenderer.h"
#include "C_Audio.h"
#include "Input.h"
#include "T_Loading.h"

#include "S_Title.h"
#include "S_Game.h"
#include "S_Result.h"

#include "Gaussian.h"
#include "MotionBlur.h"
#include "Rendpoly.h"

#include "GUIManager.h"

Scene* Manager::_scene{};
Scene* Manager::_nextscene{};
Scene* Manager::_prvscene{};
LoadingText* Manager::_loadingT;
Fade* Manager::_fade;
Rendpoly* Manager::_final;
Gaussian* Manager::_gaussian;
MotionBlur* Manager::_motionblur;
#ifdef _DEBUG
#include "CheckDoF.h"
CheckDoF* Manager::_checkdof;
#endif



void Manager::Init() {
	Input::Init();
	Renderer::Init();
	Audio::InitMaster();

	//フェード初期化
	_fade = new Fade();
	_fade->Init();

	_loadingT = new LoadingText();
	_loadingT->Init();

	_scene = new Title();
	_scene->Init();

	_final = new Rendpoly();
	_final->Init();

	_gaussian = new Gaussian();
	_gaussian->Init();

	_motionblur = new MotionBlur();
	_motionblur->Init();

#ifdef _DEBUG
	_checkdof = new CheckDoF();
	_checkdof->Init();
#endif
}


void Manager::Uninit() {
#ifdef _DEBUG
	_checkdof->Uninit();
	delete _checkdof;
#endif
	_motionblur->Uninit();
	delete _motionblur;

	_gaussian->Uninit();
	delete _gaussian;

	_final->Uninit();
	delete _final;

	_scene->Uninit();
	delete _scene;

	_loadingT->Uninit();
	delete _loadingT;

	//フェード解放
	_fade->Uninit();
	delete _fade;

	//使用していたリソース解放
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update() {
	Input::Update();

	if (_fade->GetFadeMode() == FadeMode::None) {
		_scene->Update();
	}
	_gaussian->Update();
	_motionblur->Update();
	_final->Update();
#ifdef _DEBUG
	_checkdof->Update();
#endif
	FadeUpdate();
}

void Manager::Draw() {
	//オブジェクトの描画、深度マップの作成、速度マップの作成
	Renderer::BeginPE();
	_scene->Draw();

	//ガウシアンブラーを掛ける
	_gaussian->Draw();

	//モーションブラーを掛ける
	Renderer::BeginMotionBlur();
	_motionblur->Draw();

	//最終的な描画
	Renderer::Begin();
	_final->Draw();

	//被写界深度範囲確認用
#ifdef _DEBUG
	_checkdof->Draw();
#endif

	//フェードの描画
	if (_fade->GetFadeMode() != FadeMode::None) {
		_fade->Draw();
		_loadingT->Draw();
	}


	Renderer::End();

	//画面遷移
	if (!_nextscene) {
		return;
	}
	//次のシーンがセットされていたら
	if (_scene) {
		_scene->Uninit();
		delete _scene;
		_scene = nullptr;
		GUIManager::Clear();
	}

	//シーンで利用していたリソース解放
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	_fade->LoadTexture();

	//次のシーンをセット
	_scene = _nextscene;
	_scene->Init();

	_nextscene = nullptr;
}

void Manager::FadeUpdate() {
	_fade->Update();

	_loadingT->SetFadealpha(_fade->GetFadeColor().w);
	_loadingT->Update();
}
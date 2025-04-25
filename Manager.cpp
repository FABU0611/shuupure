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

#include "ErrorHandler.h"
#include "ShaderManager.h"
#include "GUIManager.h"
#include "TextManager.h"
#include "TextureManager.h"
#include "TakePicture.h"
#include "CheckTexture.h"

float Manager::WORLD_RAD = 240.0f;

Scene* Manager::_scene{};
Scene* Manager::_nextscene{};
Scene* Manager::_prvscene{};
LoadingText* Manager::_loadingT;
Fade* Manager::_fade;
Rendpoly* Manager::_final;
Gaussian* Manager::_gaussian;
MotionBlur* Manager::_motionblur;
bool Manager::_isdrawfromlight;
int Manager::_cascadeidx = 0;



void Manager::Init() {
	Input::Init();
	Renderer::Init();
	Audio::InitMaster();
	Shader::LoadShader();

	//フェード初期化
	_fade = new Fade();
	_fade->Init();

	_loadingT = new LoadingText();
	_loadingT->Init();

	_final = new Rendpoly();
	_final->Init();

	_gaussian = new Gaussian();
	_gaussian->Init();

	_motionblur = new MotionBlur();
	_motionblur->Init();

	_scene = new Title();
	_scene->Init();

	TakePicture::Init();
}


void Manager::Uninit() {
	_scene->Uninit();
	delete _scene;

	_motionblur->Uninit();
	delete _motionblur;

	_gaussian->Uninit();
	delete _gaussian;

	_final->Uninit();
	delete _final;

	_loadingT->Uninit();
	delete _loadingT;

	//フェード解放
	_fade->Uninit();
	delete _fade;

	TextManager::GetInstance()->DeleteInstance();

	//使用していたリソース解放
	GUIManager::GetInstance()->DeleteInstance();
	TextureManager::GetInstance()->DeleteInstance();
	ErrorHandler::GetInstance()->DeleteInstance();
	Shader::UninitAll();
	ModelRenderer::UnloadAll();
	Audio::UninitAll();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update() {
	Input::Update();

	if (_fade->GetFadeMode() == FadeMode::None) {
		_scene->Update();
		GUIManager::GetInstance()->Update();
		TextManager::GetInstance()->Update();
		_gaussian->Update();
		_motionblur->Update();
		_final->Update();
	}

	FadeUpdate();
}

void Manager::Draw() {
	//ライトからの深度マップを作成
	_isdrawfromlight = true;
	for (int i = 0; i < Renderer::CASCADE_NUM; i++) {
		Renderer::BeginLightDepth(i);
		_cascadeidx = i;
		_scene->DrawFromLight();	//カメラ以外のオブジェクト描画
	}


	//オブジェクトの描画、深度マップの作成、速度マップの作成
	Renderer::BeginPE();
	_isdrawfromlight = false;
	_scene->Draw();	

	//モーションブラーを掛ける
	Renderer::BeginMotionBlur();
	_motionblur->Draw();

	//シーンにガウシアンブラーを掛ける
	_gaussian->Draw();


	//最終的な描画
	Renderer::Begin();
	_final->Draw();

	TakePicture::TakePic();

	//UIのみ描画
	_scene->DrawUI();

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
		TextManager::GetInstance()->Uninit();
		GUIManager::GetInstance()->Uninit();
		_scene->Uninit();
		delete _scene;
		_scene = nullptr;
	}

	//シーンで利用していたリソース解放
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	//TextureManager::GetInstance()->Uninit();

	_fade->Uninit();
	_fade->Init();

	//次のシーンをセット
	_scene = _nextscene;
	_scene->Init();

	_gaussian->SetSlider();
	TakePicture::Init();

	_nextscene = nullptr;
}

void Manager::FadeUpdate() {
	_fade->Update();

	_loadingT->SetFadealpha(_fade->GetFadeColor().w);
	_loadingT->Update();
}
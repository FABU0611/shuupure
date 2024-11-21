#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "C_ModelRenderer.h"
#include "C_Audio.h"
#include "Input.h"
#include "Fade.h"

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
Sprite2D* Manager::_sprite;
FadeMode Manager::_mode;
XMFLOAT4 Manager::_fadecolor;
float Manager::_faderate;
FontData* Manager::_fontdata;
DirectWriteCustomFont* Manager::_loadT;
std::string Manager::_loading;
float Manager::_time;
Fade* Manager::_fade;
Rendpoly* Manager::_final;
Gaussian* Manager::_gaussian;
MotionBlur* Manager::_motionblur;
#ifdef _DEBUG
#include "CheckDoF.h"
CheckDoF* Manager::_checkdof;
#endif



void Manager::Init()
{
	Input::Init();
	Renderer::Init();
	Audio::InitMaster();

	//フェード初期化
	_fade = new Fade();
	_fade->Init();

	_mode = FadeMode::None;
	_fadecolor = { 1.0f, 1.0f, 1.0f, 0.0f };


	_fontdata = new FontData();
	_fontdata->fontSize = 30.0f;
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Black);
	_fontdata->font = L"クラフト明朝";

	_loadT = new DirectWriteCustomFont(_fontdata);

	_loadT->Init(Renderer::GetSwapChain());
	_loadT->FontLoader();

	// 日本語ロケールのフォント名を取得
	_loadT->GetFontFamilyName(_loadT->fontCollection.Get());

	// フォントをセット
	_loadT->SetFont((*_fontdata));

	_time = 0.0f;

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


void Manager::Uninit()
{
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

	//フェード解放
	_fade->Uninit();
	delete _fade;

	//モデル解放
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if (_mode == FadeMode::None) {
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
	//オブジェクトの描画、深度値の取得
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
	if (_mode != FadeMode::None) {
		_fade->Draw();
		_loadT->DrawString(_loading, XMFLOAT2(10.0f, 10.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
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

	//モデル解放
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	_fade->Init();

	//次のシーンをセット
	_scene = _nextscene;
	_scene->Init();

	_nextscene = nullptr;
}

void Manager::FadeUpdate() {
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
			_nextscene = _prvscene;

			_prvscene = nullptr;
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
	_fade->SetColor(_fadecolor);

	_time += 0.1f;
	if (_time >= 0.2f) {
		_loading = "L";
	}
	if (_time >= 0.4f) {
		_loading = "Lo";
	}
	if (_time >= 0.6f) {
		_loading = "Loa";
	}
	if (_time >= 0.8f) {
		_loading = "Load";
	}
	if (_time >= 1.0f) {
		_loading = "Loadi";
	}
	if (_time >= 1.2f) {
		_loading = "Loadin";
	}
	if (_time >= 1.4f) {
		_loading = "Loading";
	}
	if (_time >= 1.6f) {
		_time = 0.0f;
		_loading = "";
	}
	_fontdata->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, _fadecolor.w);
	_loadT->SetFont((*_fontdata));
}
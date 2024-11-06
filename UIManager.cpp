//UIManager.h
//20106_田中　蓮
//24_09_14
#include "UIManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Manager.h"
#include "C_Audio.h"
#include "Time.h"
#include "EasingFunc.h"
#include <format>

void UIManager::Init(){
	_fontdata = new FontData();
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Red);
	_fontdata->font = L"クラフト明朝";

	_jump = new DirectWriteCustomFont(_fontdata);
	_rotaion = new DirectWriteCustomFont(_fontdata);
	_twist = new DirectWriteCustomFont(_fontdata);

	_jump->Init(Renderer::GetSwapChain());
	_jump->FontLoader();
	_rotaion->Init(Renderer::GetSwapChain());
	_rotaion->FontLoader();
	_twist->Init(Renderer::GetSwapChain());
	_twist->FontLoader();

	// 日本語ロケールのフォント名を取得
	_jump->GetFontFamilyName(_jump->fontCollection.Get());
	_rotaion->GetFontFamilyName(_rotaion->fontCollection.Get());
	_twist->GetFontFamilyName(_twist->fontCollection.Get());

	// フォントをセット
	_jump->SetFont((*_fontdata));
	_rotaion->SetFont((*_fontdata));
	_twist->SetFont((*_fontdata));

	AddComponent<Audio>(this);
	GetComponent<Audio>()->Load("asset\\audio\\clap01.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\clap01.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\clap02.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\clap02.wav", 0.5f);

	Scene* scene;
	scene = Manager::GetScene();

	_player = scene->GetGameobject<Player>();

	_size = 40.0f;
	_endsize = 60.0f;
}

void UIManager::Uninit(){
	delete _jump;
	delete _rotaion;
	delete _twist;
	delete _fontdata;
}

void UIManager::Update(){
	if (!_player) {
		return;
	}
	_state = _player->GetPlayerState();
}

void UIManager::Draw(){
	if (!_state) {
		return;
	}

	//色とテキスト変更
	std::string judge;

	if (_rot >= 0 && _rot < 3) {
		judge = "nice";
		_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Gray);
	}
	else if (_rot >= 3 && _rot < 5) {
		judge = "Good";
		_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Magenta);
	}
	else if (_rot >= 5 && _rot < 10) {
		judge = "Wow!";
		_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Aqua);
	}
	else if (_rot >= 10) {
		judge = "Excellent!!";
		_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Yellow);
	}


	//フォント更新
	_rotaion->SetFont((*_fontdata));
	
	//描画
	_rotaion->DrawString(judge, XMFLOAT2(750, 300), D2D1_DRAW_TEXT_OPTIONS_NONE);
}

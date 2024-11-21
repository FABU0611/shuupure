//TitlePush.cpp
//20106_田中　蓮
//24_09_15
#include "TitlePush.h"
#include "Manager.h"
#include "Renderer.h"
#include "S_Game.h"
#include "S_Tutorial.h"
#include "Input.h"
#include "C_Audio.h"
#include "EasingFunc.h"
#include "Time.h"
#include "G_Button.h"
#include "GUIManager.h"

void TitlePush::Init(){
	_fontdata = new FontData();
	_fontdata->fontSize = 60.0f;
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Yellow);
	_fontdata->font = L"クラフト明朝";

	_push = new DirectWriteCustomFont(_fontdata);

	_push->Init(Renderer::GetSwapChain());
	_push->FontLoader();

	// 日本語ロケールのフォント名を取得
	_push->GetFontFamilyName(_push->fontCollection.Get());

	// フォントをセット
	_push->SetFont((*_fontdata));

	AddComponent<Audio>(this);
	GetComponent<Audio>()->Load("asset\\audio\\push.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\push.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\bgm002.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\bgm002.wav", 0.2f);
	GetComponent<Audio>()->Play("asset\\audio\\bgm002.wav", false);

	_spos.x = _epos.x = _drawpos.x = (SCREEN_WIDTH * 0.5f) - 190.0f;
	_spos.y = SCREEN_HEIGHT + 100.0f;
	_epos.y = 600.0f;

	_start = GUIManager::AddGUI<Button>(UI, XMFLOAT3(200.0f, 200.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f));
}

void TitlePush::Uninit(){
}

void TitlePush::Update(){
	//フォントのアニメーション
	float t = _pos;
	_pos += Time::GetDeltaTime();
	_drawpos.y = std::lerp(_spos.y, _epos.y, Easing::EaseOutBounce(t));

	if (t > 1.0f) {
		_pos = 1.0f;
		_drawpos.y = _epos.y;
	}
	Button* button = dynamic_cast<Button*>(GUIManager::GetGUI(_start));
	if (!button) {
		return;
	}
	//シーン遷移
	if (Input::GetKeyTrigger('K') || button->OnClicked(VK_LBUTTON)) {
		GetComponent<Audio>()->Play("asset\\audio\\push.wav", false);
		Manager::SetSceneFade<Tutorial>(0.05f);
	}
}

void TitlePush::Draw(){
	_push->DrawString("Push K key", _drawpos, D2D1_DRAW_TEXT_OPTIONS_NONE);
}

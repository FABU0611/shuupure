//TutorialText.cpp
//20106_田中　蓮
//24_09_17
#include "T_Tutorial.h"
#include "Renderer.h"

#include "EasingFunc.h"
#include "Time.h"
#include <format>

void TutorialText::Init(){
	_fontdata = new FontData();
	_fontdata->fontSize = 30.0f;
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Black);
	_fontdata->font = L"クラフト明朝";

	_tutoT = new DirectWriteCustomFont(_fontdata);

	_tutoT->Init(Renderer::GetSwapChain());
	_tutoT->FontLoader();

	// 日本語ロケールのフォント名を取得
	_tutoT->GetFontFamilyName(_tutoT->fontCollection.Get());

	// フォントをセット
	_tutoT->SetFont((*_fontdata));

	_spos.x = _epos.x = _drawpos.x = 90.0f;
	_spos.y = 90.0f;
	_epos.y = 90.0f;
}

void TutorialText::Uninit() {
}

void TutorialText::Update(){
	//上下
	_spos.y += Time::GetDeltaTime();
	_drawpos.y = cos(_spos.y * 2) + _epos.y;

	_tutoT->SetFont((*_fontdata));
}

void TutorialText::Draw(){
	_tutoT->DrawString("回転\tW,S Key\n"
						"捻り\tA,D Kry\n\n"
						"カメラ回転\tI,J,K,L Key\n\n"
						"ゲーム開始\tSpace Key",
						_drawpos, D2D1_DRAW_TEXT_OPTIONS_NONE);
}

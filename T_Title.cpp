//Title.cpp
//20106_田中　蓮
//24_09_15
#include "T_Title.h"
#include "Renderer.h"

#include "EasingFunc.h"
#include "Time.h"
void TitleText::Init(){
	_fontdata = new FontData();
	_fontdata->fontSize = 120.0f;
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = D2D1::ColorF(D2D1::ColorF::GreenYellow);
	_fontdata->font = L"クラフト明朝";

	_title = new DirectWriteCustomFont(_fontdata);

	_title->Init(Renderer::GetSwapChain());
	_title->FontLoader();

	// 日本語ロケールのフォント名を取得
	_title->GetFontFamilyName(_title->fontCollection.Get());

	// フォントをセット
	_title->SetFont((*_fontdata));

	_spos.x = _epos.x = _drawpos.x = (SCREEN_WIDTH * 0.5f) - 330.0f;
	_spos.y = 100.0f;
	_epos.y = 100.0f;
}

void TitleText::Uninit(){
}

void TitleText::Update(){
	//上下
	_spos.y += Time::GetDeltaTime();
	_drawpos.y = cos(_spos.y * 2) + _epos.y;

	_title->SetFont((*_fontdata));
}

void TitleText::Draw(){
	_title->DrawString("Spin Dive", _drawpos, D2D1_DRAW_TEXT_OPTIONS_NONE);
}

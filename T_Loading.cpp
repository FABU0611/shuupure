//T_Loading.cpp
//20106_田中　蓮
//24_11_21
#include "T_Loading.h"
#include "Renderer.h"

void LoadingText::Init() {
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
	_drawpos = { 10.0f, 10.0f };
}

void LoadingText::Uninit() {}

void LoadingText::Update() {
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
	_fontdata->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, _fadealpha);
	_loadT->SetFont((*_fontdata));
}

void LoadingText::Draw() {
	_loadT->DrawString(_loading, _drawpos, D2D1_DRAW_TEXT_OPTIONS_NONE);
}

//Text.cpp
//20106_田中　蓮
//24_11_26
#include "Text.h"
#include "Renderer.h"

void Text::Init() {
	_text = new DirectWriteCustomFont(&_fontdata);
	_text->Init(Renderer::GetSwapChain());
	_text->GetFontFamilyName(_text->fontCollection.Get(), L"ja-JP");

	_fontdata.fontSize = _size;
	_fontdata.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata.Color = _color;
	_fontdata.font = L"クラフト明朝";

	//フォントをセット
	_text->SetFont(_fontdata);
}

void Text::Uninit() {
	if (!_text) {
		return;
	}
	_text->Uninit();  // リソースを解放
	delete _text;     // メモリを解放
	_text = nullptr;  // 二重解放防止
}

void Text::Update() {}

void Text::Draw() {
	if (!_text) {
		return;
	}
	XMFLOAT2 pos = { GetPosition().x, GetPosition().y };
	_text->DrawString(_str, pos, D2D1_DRAW_TEXT_OPTIONS_NONE, _anchor);
}

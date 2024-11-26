//Text.cpp
//20106_田中　蓮
//24_11_26
#include "Text.h"
#include "Renderer.h"

void Text::Init() {
	_fontdata = new FontData();

	_text = new DirectWriteCustomFont(_fontdata);
	_text->Init(Renderer::GetSwapChain());
	//日本語ロケールのフォント名を取得
	_text->GetFontFamilyName(_text->fontCollection.Get(), L"ja-JP");

	_fontdata->fontSize = _size;
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = _color;
	_fontdata->font = L"クラフト明朝";

	//フォントをセット
	_text->SetFont((*_fontdata));
}

void Text::Uninit() {}

void Text::Update() {}

void Text::Draw() {
	XMFLOAT2 pos = { GetPosition().x, GetPosition().y };
	_text->DrawString(_str, pos, D2D1_DRAW_TEXT_OPTIONS_NONE);
}

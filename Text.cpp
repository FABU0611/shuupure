//Text.cpp
//20106_�c���@�@
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
	_fontdata.font = L"�N���t�g����";

	//�t�H���g���Z�b�g
	_text->SetFont(_fontdata);
}

void Text::Uninit() {
	if (!_text) {
		return;
	}
	_text->Uninit();  // ���\�[�X�����
	delete _text;     // �����������
	_text = nullptr;  // ��d����h�~
}

void Text::Update() {}

void Text::Draw() {
	if (!_text) {
		return;
	}
	XMFLOAT2 pos = { GetPosition().x, GetPosition().y };
	_text->DrawString(_str, pos, D2D1_DRAW_TEXT_OPTIONS_NONE, _anchor);
}

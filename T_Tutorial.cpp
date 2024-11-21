//TutorialText.cpp
//20106_�c���@�@
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
	_fontdata->font = L"�N���t�g����";

	_tutoT = new DirectWriteCustomFont(_fontdata);

	_tutoT->Init(Renderer::GetSwapChain());
	_tutoT->FontLoader();

	// ���{�ꃍ�P�[���̃t�H���g�����擾
	_tutoT->GetFontFamilyName(_tutoT->fontCollection.Get());

	// �t�H���g���Z�b�g
	_tutoT->SetFont((*_fontdata));

	_spos.x = _epos.x = _drawpos.x = 90.0f;
	_spos.y = 90.0f;
	_epos.y = 90.0f;
}

void TutorialText::Uninit() {
}

void TutorialText::Update(){
	//�㉺
	_spos.y += Time::GetDeltaTime();
	_drawpos.y = cos(_spos.y * 2) + _epos.y;

	_tutoT->SetFont((*_fontdata));
}

void TutorialText::Draw(){
	_tutoT->DrawString("��]\tW,S Key\n"
						"�P��\tA,D Kry\n\n"
						"�J������]\tI,J,K,L Key\n\n"
						"�Q�[���J�n\tSpace Key",
						_drawpos, D2D1_DRAW_TEXT_OPTIONS_NONE);
}

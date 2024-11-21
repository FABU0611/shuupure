//T_Score.cpp
//20106_�c���@�@
//24_09_15
#include "T_Score.h"
#include "Renderer.h"

#include "EasingFunc.h"
#include "Time.h"
#include "DataManager.h"
#include <format>

void ScoreText::Init(){
	_fontdata = new FontData();
	_fontdata->fontSize = 80.0f;
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Black);
	_fontdata->font = L"�N���t�g����";

	_scoreT = new DirectWriteCustomFont(_fontdata);
	_dotT = new DirectWriteCustomFont(_fontdata);
	_backT = new DirectWriteCustomFont(_fontdata);

	_scoreT->Init(Renderer::GetSwapChain());
	_scoreT->FontLoader();
	_dotT->Init(Renderer::GetSwapChain());
	_dotT->FontLoader();
	_backT->Init(Renderer::GetSwapChain());
	_backT->FontLoader();

	// ���{�ꃍ�P�[���̃t�H���g�����擾
	_scoreT->GetFontFamilyName(_scoreT->fontCollection.Get());
	_dotT->GetFontFamilyName(_scoreT->fontCollection.Get());
	_backT->GetFontFamilyName(_scoreT->fontCollection.Get());

	// �t�H���g���Z�b�g
	_scoreT->SetFont((*_fontdata));
	_dotT->SetFont((*_fontdata));
	_backT->SetFont((*_fontdata));

	_spos.x = _epos.x = _drawpos.x = 190.0f;
	_spos.y = _drawpos.y = SCREEN_HEIGHT + 100.0f;
	_epos.y = 100.0f;


	//�Z�[�u�f�[�^�����[�h����
	DataManager::LoadData(_rot, _twist, _dot);
	DataManager::LoadScore(_curscore);
	//�X�R�A�̌v�Z
	_dot *= 10;
	_score += _dot;
	_score += _rot * 10;
	_score += _twist * 5;
}

void ScoreText::Uninit() {
	if (_score > _curscore) {
		DataManager::SaveScore(_score);
	}
}

void ScoreText::Update(){
	//�t�H���g�̃A�j���[�V����
	float t = _pos;
	_time += Time::GetDeltaTime();
	_pos += Time::GetDeltaTime();
	_drawpos.y = std::lerp(_spos.y, _epos.y, Easing::EaseOutExpo(t));

	if (t > 1.0f) {
		_pos = 1.0f;
		_drawpos.y = _epos.y;
	}

	if (_time > 1.0f) {
		_alpha = 0.0f;
	}
	if (_time > 2.0f) {
		_alpha = 1.0f;
		_time = 0.0f;
	}
}

void ScoreText::Draw(){
	_dotT->DrawString("�����p�x\t" + std::format("{:.0f}", _dot) + "�_\n"
						"��]��\t" + std::format("{:d}", _twist) + "��\n"
						"�P���\t" + std::format("{:d}", _rot) + "��\n\n"
						"Score\t\t" + std::format("{:.0f}", _score),
						_drawpos, D2D1_DRAW_TEXT_OPTIONS_NONE);


	_fontdata->Color = D2D1::ColorF(0.0f, 1.0f, 0.0f, _alpha);
	_fontdata->fontSize = 50.0f;
	_backT->SetFont((*_fontdata));
	_backT->DrawString("K key Back Title", XMFLOAT2((SCREEN_WIDTH * 0.5f) - 250.0f, 600.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);

	if (_curscore < _score && _pos >= 1.0f) {
		_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Gold);
		_fontdata->fontSize = 40.0f;
		_scoreT->SetFont((*_fontdata));
		_scoreT->DrawString("New Record!!", XMFLOAT2(900.0f, 400.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
}

#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "C_ModelRenderer.h"
#include "C_Audio.h"
#include "Input.h"
#include "Fade.h"

#include "S_Title.h"
#include "S_Game.h"
#include "S_Result.h"

#include "Gaussian.h"
#include "MotionBlur.h"
#include "Rendpoly.h"

#include "GUIManager.h"

Scene* Manager::_scene{};
Scene* Manager::_nextscene{};
Scene* Manager::_prvscene{};
Sprite2D* Manager::_sprite;
FadeMode Manager::_mode;
XMFLOAT4 Manager::_fadecolor;
float Manager::_faderate;
FontData* Manager::_fontdata;
DirectWriteCustomFont* Manager::_loadT;
std::string Manager::_loading;
float Manager::_time;
Fade* Manager::_fade;
Rendpoly* Manager::_final;
Gaussian* Manager::_gaussian;
MotionBlur* Manager::_motionblur;
#ifdef _DEBUG
#include "CheckDoF.h"
CheckDoF* Manager::_checkdof;
#endif



void Manager::Init()
{
	Input::Init();
	Renderer::Init();
	Audio::InitMaster();

	//�t�F�[�h������
	_fade = new Fade();
	_fade->Init();

	_mode = FadeMode::None;
	_fadecolor = { 1.0f, 1.0f, 1.0f, 0.0f };


	_fontdata = new FontData();
	_fontdata->fontSize = 30.0f;
	_fontdata->fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
	_fontdata->Color = D2D1::ColorF(D2D1::ColorF::Black);
	_fontdata->font = L"�N���t�g����";

	_loadT = new DirectWriteCustomFont(_fontdata);

	_loadT->Init(Renderer::GetSwapChain());
	_loadT->FontLoader();

	// ���{�ꃍ�P�[���̃t�H���g�����擾
	_loadT->GetFontFamilyName(_loadT->fontCollection.Get());

	// �t�H���g���Z�b�g
	_loadT->SetFont((*_fontdata));

	_time = 0.0f;

	_scene = new Title();
	_scene->Init();

	_final = new Rendpoly();
	_final->Init();

	_gaussian = new Gaussian();
	_gaussian->Init();

	_motionblur = new MotionBlur();
	_motionblur->Init();

#ifdef _DEBUG
	_checkdof = new CheckDoF();
	_checkdof->Init();
#endif
}


void Manager::Uninit()
{
#ifdef _DEBUG
	_checkdof->Uninit();
	delete _checkdof;
#endif
	_motionblur->Uninit();
	delete _motionblur;

	_gaussian->Uninit();
	delete _gaussian;

	_final->Uninit();
	delete _final;

	_scene->Uninit();
	delete _scene;

	//�t�F�[�h���
	_fade->Uninit();
	delete _fade;

	//���f�����
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if (_mode == FadeMode::None) {
		_scene->Update();
	}
	_gaussian->Update();
	_motionblur->Update();
	_final->Update();
#ifdef _DEBUG
	_checkdof->Update();
#endif
	FadeUpdate();
}

void Manager::Draw() {
	//�I�u�W�F�N�g�̕`��A�[�x�l�̎擾
	Renderer::BeginPE();
	_scene->Draw();

	//�K�E�V�A���u���[���|����
	_gaussian->Draw();

	//���[�V�����u���[���|����
	Renderer::BeginMotionBlur();
	_motionblur->Draw();

	//�ŏI�I�ȕ`��
	Renderer::Begin();
	_final->Draw();

	//��ʊE�[�x�͈͊m�F�p
#ifdef _DEBUG
	_checkdof->Draw();
#endif

	//�t�F�[�h�̕`��
	if (_mode != FadeMode::None) {
		_fade->Draw();
		_loadT->DrawString(_loading, XMFLOAT2(10.0f, 10.0f), D2D1_DRAW_TEXT_OPTIONS_NONE);
	}


	Renderer::End();

	//��ʑJ��
	if (!_nextscene) {
		return;
	}
	//���̃V�[�����Z�b�g����Ă�����
	if (_scene) {
		_scene->Uninit();
		delete _scene;
		_scene = nullptr;
		GUIManager::Clear();
	}

	//���f�����
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	_fade->Init();

	//���̃V�[�����Z�b�g
	_scene = _nextscene;
	_scene->Init();

	_nextscene = nullptr;
}

void Manager::FadeUpdate() {
	if (_mode == FadeMode::None) {
		return;
	}
	//�t�F�[�h�A�E�g����
	if (_mode == FadeMode::Out) {
		_fadecolor.w += _faderate;

		if (_fadecolor.w >= 1.0f) {
			//�t�F�[�h�C�������ɐ؂�ւ�
			_fadecolor.w = 1.0f;
			_mode = FadeMode::In;

			//���[�h��ݒ�
			_nextscene = _prvscene;

			_prvscene = nullptr;
		}
	}
	//�t�F�[�h�C������
	else if (_mode == FadeMode::In) {
		_fadecolor.w -= _faderate;

		if (_fadecolor.w <= 0.0f) {
			//�t�F�[�h�����I��
			_fadecolor.w = 0.0f;
			_mode = FadeMode::None;
		}
	}
	//�X�v���C�g�̐F�X�V
	_fade->SetColor(_fadecolor);

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
	_fontdata->Color = D2D1::ColorF(0.0f, 0.0f, 0.0f, _fadecolor.w);
	_loadT->SetFont((*_fontdata));
}
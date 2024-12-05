#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "C_ModelRenderer.h"
#include "C_Audio.h"
#include "Input.h"
#include "T_Loading.h"

#include "S_Title.h"
#include "S_Game.h"
#include "S_Result.h"

#include "Gaussian.h"
#include "MotionBlur.h"
#include "Rendpoly.h"

#include "GUIManager.h"
#include "TextManager.h"

Scene* Manager::_scene{};
Scene* Manager::_nextscene{};
Scene* Manager::_prvscene{};
LoadingText* Manager::_loadingT;
Fade* Manager::_fade;
Rendpoly* Manager::_final;
Gaussian* Manager::_gaussian;
MotionBlur* Manager::_motionblur;
GUIManager* Manager::_guimanager;
TextManager* Manager::_textmanager;

#ifdef _DEBUG
#include "CheckDoF.h"
CheckDoF* Manager::_checkdof;
#endif



void Manager::Init() {
	Input::Init();
	Renderer::Init();
	Audio::InitMaster();

	_guimanager = new GUIManager();

	_textmanager = new TextManager();

	//�t�F�[�h������
	_fade = new Fade();
	_fade->Init();

	_loadingT = new LoadingText();
	_loadingT->Init();

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


void Manager::Uninit() {
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

	_loadingT->Uninit();
	delete _loadingT;

	//�t�F�[�h���
	_fade->Uninit();
	delete _fade;

	_textmanager->Uninit();
	delete _textmanager;

	_guimanager->Uninit();
	delete _guimanager;

	//�g�p���Ă������\�[�X���
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	Audio::UninitMaster();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update() {
	Input::Update();

	if (_fade->GetFadeMode() == FadeMode::None) {
		_scene->Update();
		_guimanager->Update();
		_textmanager->Update();
		_gaussian->Update();
		_motionblur->Update();
		_final->Update();
#ifdef _DEBUG
		_checkdof->Update();
#endif
	}

	FadeUpdate();
}

void Manager::Draw() {
	//�I�u�W�F�N�g�̕`��A�[�x�}�b�v�̍쐬�A���x�}�b�v�̍쐬
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

	if (Input::GetKeyTrigger('T')) {
		Renderer::TakeingPic();
	}

	//GUI�`��
	_guimanager->Draw();

	//�e�L�X�g�̂ݕ`��
	_textmanager->Draw();
	
	//��ʊE�[�x�͈͊m�F�p
#ifdef _DEBUG
	_checkdof->Draw();
#endif

	//�t�F�[�h�̕`��
	if (_fade->GetFadeMode() != FadeMode::None) {
		_fade->Draw();
		_loadingT->Draw();
	}


	Renderer::End();

	//��ʑJ��
	if (!_nextscene) {
		return;
	}
	//���̃V�[�����Z�b�g����Ă�����
	if (_scene) {
#ifdef _DEBUG
		_checkdof->Uninit();
#endif
		_textmanager->Uninit();
		_guimanager->Uninit();
		_scene->Uninit();
		delete _scene;
		_scene = nullptr;
	}

	//�V�[���ŗ��p���Ă������\�[�X���
	ModelRenderer::UnloadAll();
	Audio::UninitAll();
	Sprite::UninitAll();

	_fade->Uninit();
	_fade->Init();

	//���̃V�[�����Z�b�g
	_scene = _nextscene;
	_scene->Init();

#ifdef _DEBUG
	_checkdof->Init();
#endif

	_nextscene = nullptr;
}

void Manager::FadeUpdate() {
	_fade->Update();

	_loadingT->SetFadealpha(_fade->GetFadeColor().w);
	_loadingT->Update();
}
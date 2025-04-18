//Gaussian.h
//20106_田中　蓮
//24_11_07
#include "Gaussian.h"
#include "Renderer.h"
#include "GaussianX.h"
#include "GaussianY.h"
#include "Input.h"
#include "Manager.h"
#include "GUIManager.h"
#include "G_Slidebar.h"
#include "Scene.h"
#include "S_Title.h"

void Gaussian::CalcGaussianweight(float dispersion) {
	float total = 0;
	for (int i = 0; i < 8; i++) {
		float r = 1.0f + 2.0f * i;
		_weight[i] = expf(-0.5f * (float)(r * r) / dispersion);

		if (i == 0) {
			//代表のピクセルウェイト
			total += _weight[i];
		}
		else {
			//周辺のピくエルは２回分行う
			total += 2.0f * _weight[i];
		}
	}

	//ウェイトの合計が１になるように
	for (int i = 0; i < 8; i++) {
		_weight[i] /= total;
	}
}

void Gaussian::Init() {
	_gaussianx = new GaussianX();
	_gaussianx->Init();
	_gaussiany = new GaussianY();
	_gaussiany->Init();

	SetSlider();
}

void Gaussian::Uninit() {
	if (_gaussiany) {
		_gaussiany->Uninit();
	}
	if (_gaussianx) {
		_gaussianx->Uninit();
	}
}

void Gaussian::Update() {
	if (Input::GetKeyPress('1')) {
		_param.z += -0.04f;
		_param.z = (std::max)(_param.z, 0.01f);
	}
	if (Input::GetKeyPress('2')) {
		_param.z += 0.04f;
		_param.z = (std::min)(_param.z, 10.0f);
	}
	CalcGaussianweight(_param.z);

	if (Input::GetKeyPress('3')) {			//奥
		_dof.x += 0.001f;
		_dof.x = (std::min)(_dof.x, 2.0f);
	}
	if (Input::GetKeyPress('4')) {			//手前
		_dof.x += -0.001f;
		_dof.x = (std::max)(_dof.x, 0.0f);
	}
	if (Input::GetKeyPress('5')) {			//広く
		_dof.y += 0.001f;
		_dof.y = (std::min)(_dof.y, 10.0f);
	}
	if (Input::GetKeyPress('6')) {			//狭く
		_dof.y += -0.001f;
		_dof.y = (std::max)(_dof.y, 0.01f);
	}
}

void Gaussian::Draw() {
	Renderer::SetParameter(_param);
	Renderer::SetWeights(&_weight[0]);
	Renderer::SetDoF(_dof);

	Renderer::BeginBlurX();
	_gaussianx->Draw();
	Renderer::BeginBlurY();
	_gaussiany->Draw();
}

void Gaussian::SetSlider() {
	GUIManager::GetInstance()->AddGUI<Slidebar>("ボケ具合", XMFLOAT3(10.0f, 470.0f, 0.0f), 10.0f, 0.01f, &_param.z);
	GUIManager::GetInstance()->AddGUI<Slidebar>("ボケ位置", XMFLOAT3(10.0f, 520.0f, 0.0f), 2.0f, 0.0f, &_dof.x);
	GUIManager::GetInstance()->AddGUI<Slidebar>("ボケ範囲", XMFLOAT3(10.0f, 570.0f, 0.0f), 1.0f, 0.01f, &_dof.y);
}

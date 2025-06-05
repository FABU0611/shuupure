//Gaussian.h
//20106_田中　蓮
//24_11_07
#include "Gaussian.h"
#include "Renderer.h"
#include "GaussianX.h"
#include "GaussianY.h"
#include "Input.h"
#include "Time.h"
#include "Manager.h"
#include "GUIManager.h"
#include "G_Slidebar.h"

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
		delete _gaussiany;
	}
	if (_gaussianx) {
		_gaussianx->Uninit();
		delete _gaussianx;
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

void Gaussian::Draw(ID3D11ShaderResourceView* inputSRV) {
	Renderer::SetParameter(_param);
	Renderer::SetWeights(&_weight[0]);
	Renderer::SetDoF(_dof);

	Renderer::BeginBlurX();
	_gaussianx->Draw(inputSRV);
	Renderer::BeginBlurY();
	_gaussiany->Draw(_gaussianx->GetWriteSRV());
}

void Gaussian::SetSlider() {
	GUIManager::GetInstance()->AddGUI<Slidebar>("ボケ具合", XMFLOAT3(10.0f, 470.0f, 0.0f), 10.0f, 0.01f, &_param.z);
	GUIManager::GetInstance()->AddGUI<Slidebar>("ボケ位置", XMFLOAT3(10.0f, 520.0f, 0.0f), 2.0f, 0.0f, &_dof.x);
	GUIManager::GetInstance()->AddGUI<Slidebar>("ボケ範囲", XMFLOAT3(10.0f, 570.0f, 0.0f), 1.0f, 0.01f, &_dof.y);
}

bool Gaussian::LerpDoF(const XMFLOAT2& dof, const float& time) {
	if (dof == _dof) {
		return true;
	}
	if (!_isdoflerping) {
		_startdof = _dof;
		_isdoflerping = true;
	}

	_doftime += Time::GetDeltaTime();
	float d = std::clamp(_doftime / time, 0.0f, 1.0f);

	SetDof({ std::lerp(_startdof.x, dof.x, d), std::lerp(_startdof.y, dof.y, d)} );

	if (d >= 1.0f) {
		SetDof(dof);
		_doftime = 0.0f;
		_isdoflerping = false;
		return true;
	}
	return false;
}

bool Gaussian::LerpBoke(float boke, const float& time) {
	boke = std::clamp(boke, 0.01f, 10.0f);
	if (boke == _param.z) {
		return true;
	}
	if (!_isbokelerping) {
		_startboke = _param.z;
		_isbokelerping = true;
	}

	_boketime += Time::GetDeltaTime();
	float d = std::clamp(_boketime / time, 0.0f, 1.0f);

	SetBoke(std::lerp(_startboke, boke, d));

	if (d >= 1.0f) {
		SetBoke(boke);
		_boketime = 0.0f;
		_isbokelerping = false;
		return true;
	}
	return false;
}

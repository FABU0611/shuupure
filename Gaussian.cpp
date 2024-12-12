//Gaussian.h
//20106_�c���@�@
//24_11_07
#include "Gaussian.h"
#include "Renderer.h"
#include "GaussianX.h"
#include "GaussianY.h"
#include "Input.h"
#include "Manager.h"
#include "Scene.h"
#include "S_Title.h"

void Gaussian::CalcGaussianweight(float dispersion) {
	float total = 0;
	for (int i = 0; i < 8; i++) {
		float r = 1.0f + 2.0f * i;
		_weight[i] = expf(-0.5f * (float)(r * r) / dispersion);

		if (i == 0) {
			//��\�̃s�N�Z���E�F�C�g
			total += _weight[i];
		}
		else {
			//���ӂ̃s���G���͂Q�񕪍s��
			total += 2.0f * _weight[i];
		}
	}

	//�E�F�C�g�̍��v���P�ɂȂ�悤��
	for (int i = 0; i < 8; i++) {
		_weight[i] /= total;
	}
}

void Gaussian::Init() {
	_gaussianx = new GaussianX();
	_gaussianx->Init();
	_gaussiany = new GaussianY();
	_gaussiany->Init();

	_param.x = SCREEN_WIDTH;
	_param.y = SCREEN_HEIGHT;
	Renderer::SetParameter(_param);

	Scene* scene = Manager::GetScene();
	if (scene) {
		Title* title = dynamic_cast<Title*>(scene);
		if (title) {
			_param.z = 10.0f;
			_dof.x = 0.03f;
			_dof.y = 0.05f;
		}
	}
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

	if (Input::GetKeyPress('3')) {			//��
		_dof.x += 0.001f;
		_dof.x = (std::min)(_dof.x, 2.0f);
	}
	if (Input::GetKeyPress('4')) {			//��O
		_dof.x += -0.001f;
		_dof.x = (std::max)(_dof.x, 0.0f);
	}
	if (Input::GetKeyPress('5')) {			//�L��
		_dof.y += 0.001f;
		_dof.y = (std::min)(_dof.y, 10.0f);
	}
	if (Input::GetKeyPress('6')) {			//����
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

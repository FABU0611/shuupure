//Gaussian.h
//20106_ìcíÜÅ@ò@
//24_11_07
#pragma once
#include "Main.h"

class GaussianX;
class GaussianY;

class Gaussian {
	GaussianX*	_gaussianx{};
	GaussianY*	_gaussiany{};

	XMFLOAT4	_param = { 0.1f, 0.1f, 0.1f, 0.1f };
	float		_weight[8];
	XMFLOAT2	_dof = { 0.0f, 0.0f };

	void CalcGaussianweight(float);

public:
	Gaussian() {}
	~Gaussian() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
//Gaussian.h
//20106_田中　蓮
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
	XMFLOAT2	_dof = { 0.35f, 0.6f };

	void CalcGaussianweight(float);

public:
	Gaussian() {}
	~Gaussian() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetSlider();

	/// <summary> ピントの位置と広さを設定 </summary>
	/// <param name="x"> ピントの位置</param>
	/// <param name="y"> ピントの広さ</param>
	void SetDof(const XMFLOAT2& dof) { _dof = dof; }
	/// <summary> ボケ具合を設定 </summary>
	void SetBoke(const float& boke) { _param.z = boke; }
};
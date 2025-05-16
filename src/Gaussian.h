//Gaussian.h
//20106_�c���@�@
//24_11_07
#pragma once
#include "Main.h"

class GaussianX;
class GaussianY;

class Gaussian {
	GaussianX*	_gaussianx{};
	GaussianY*	_gaussiany{};

	XMFLOAT4	_param = { 0.1f, 0.1f, 0.1f, 0.1f };
	float		_startboke = 0.0f;
	float		_weight[8];
	XMFLOAT2	_dof = { 0.35f, 0.6f };
	XMFLOAT2	_startdof = { 0.0f, 0.0f };
	float		_doftime = 0.0f;
	float		_boketime = 0.0f;
	bool		_isdoflerping = false;
	bool		_isbokelerping = false;

	void CalcGaussianweight(float);

public:
	Gaussian() {}
	~Gaussian() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetSlider();

	/// <summary> �s���g�̈ʒu�ƍL����ݒ� </summary>
	/// <param name="x"> �s���g�̈ʒu</param>
	/// <param name="y"> �s���g�̍L��</param>
	void SetDof(const XMFLOAT2& dof) { _dof = dof; }
	/// <summary> �{�P���ݒ� </summary>
	void SetBoke(const float& boke) { _param.z = boke; }
	bool LerpDoF(const XMFLOAT2& dof, const float& time);
	bool LerpBoke(float boke, const float& time);
};
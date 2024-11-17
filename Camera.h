//camera.h
//20106_ìcíÜÅ@ò@
//24_05_08
#pragma once
#include "GameObject.h"
class Camera : public GameObject{
	XMFLOAT3		_target;
	XMFLOAT4X4		_viewmatrix;
	float			_length;
	float			_cameraspeed;

	XMMATRIX		_prevview;
	XMMATRIX		_prevprojection;

public:
	Camera() {}
	~Camera() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTarget(const XMFLOAT3& targetpos) {
		_target = targetpos;
	}

	XMVECTOR GetZDirection() const{ 
		//ãtçsóÒ
		XMMATRIX view =  XMLoadFloat4x4(&_viewmatrix); 
		return XMMatrixInverse(nullptr, view).r[2];
	}
	XMMATRIX GetViewMatrix() const{	return XMLoadFloat4x4(&_viewmatrix);	}
};


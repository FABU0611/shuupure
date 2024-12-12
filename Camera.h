//camera.h
//20106_ìcíÜÅ@ò@
//24_05_08
#pragma once
#include "GameObject.h"

class CameraState;

class Camera : public GameObject{
	XMFLOAT3		_target{};
	XMFLOAT4X4		_viewmatrix{};
	float			_length{};
	float			_cameraspeed{};
	float			_time{};

	XMMATRIX		_prevview{};
	XMMATRIX		_prevprojection{};

	CameraState*	_state{};

public:
	Camera() {}
	~Camera() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTarget(const XMFLOAT3& targetpos) { _target = targetpos; }
	void SetLength(const float& length) { _length = length; }

	XMFLOAT3& GetTartgetPos() { return _target; }

	void ChangeState(CameraState* state) {
		if (!_state) {
			return;
		}
		delete _state;
		_state = state;
	}

	XMVECTOR GetZDirection() const{ 
		//ãtçsóÒ
		XMMATRIX view =  XMLoadFloat4x4(&_viewmatrix); 
		return XMMatrixInverse(nullptr, view).r[2];
	}
	XMMATRIX GetViewMatrix() const{	return XMLoadFloat4x4(&_viewmatrix); }
};


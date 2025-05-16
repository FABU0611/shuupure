//camera.h
//20106_ìcíÜÅ@ò@
//24_05_08
#pragma once
#include "GameObject.h"
#include "Renderer.h"
#include <vector>

class Camera : public GameObject {
	static const float NEAR_CLIP;
	static const float FAR_CLIP;

	XMFLOAT3		_target{};
	XMMATRIX		_viewmatrix{};
	XMMATRIX		_projectionMatrix{};
	float			_length{};
	float			_cameraspeed{};
	float			_time{};
	float			_cascade[Renderer::CASCADE_NUM]{};
	float			_fov{};
	float			_aspect{};

	XMMATRIX		_prevview{};
	XMMATRIX		_prevprojection{};

	class CameraState*	_state{};

	void CalculationCascade();

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

	void ChangeState(CameraState* state);

	XMVECTOR GetZDirection() const{ 
		//ãtçsóÒ
		XMMATRIX view =  _viewmatrix; 
		return XMMatrixInverse(nullptr, view).r[2];
	}
	XMMATRIX GetViewMatrix() const{	return _viewmatrix; }

	std::vector<XMVECTOR> GetCornersWorldSpace(const float& nearZ, const float& farZ)const;
	float* GetCascade() { return _cascade; }
	bool ChackView(const XMFLOAT3& pos, const XMFLOAT3& scl);
	std::vector<int> CheckViewInstance(const XMFLOAT3* instancepos, const XMFLOAT3 basepos, const int& instancenum, const XMFLOAT3& scl);
};


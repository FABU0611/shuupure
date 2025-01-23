//SphereInstance.h
//20106_ìcíÜÅ@ò@
//25_01_23
#pragma once
#include "GameObject.h"

class SphereInstance : public GameObject {
	static const int	INSTANCE_X_NUM = 10;
	static const int	INSTANCE_Z_NUM = 10;
	static const int	INSTANCE_NUM = INSTANCE_X_NUM * INSTANCE_Z_NUM;
	static const float	INSTANCE_DISTANCE;

	ID3D11Buffer* _posbuffer{};
	ID3D11ShaderResourceView* _posSRV{};

	XMMATRIX			_prevworld{};

public:
	SphereInstance() {}
	~SphereInstance() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
//SphereInstance.h
//20106_�c���@�@
//25_01_23
#pragma once
#include "GameObject.h"

class SphereInstance : public GameObject {
	static const int	INSTANCE_X_NUM = 20;
	static const int	INSTANCE_Z_NUM = 20;
	static const int	INSTANCE_NUM = INSTANCE_X_NUM * INSTANCE_Z_NUM;
	static const float	INSTANCE_DISTANCE;

	ID3D11Buffer*		_posbuffer{};
	ID3D11ShaderResourceView* _posSRV{};

	XMFLOAT3*			_instancepos{};	//�C���X�^���X�̈ʒu

public:
	SphereInstance() {}
	~SphereInstance() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
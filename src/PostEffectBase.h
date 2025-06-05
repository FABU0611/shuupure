//PostEffectBase.h
//20106_�c���@�@
//25_05_24
#pragma once
#include "GameObject.h"

/// <summary>
/// Draw���]�b�g�̒���SRV���e�X�ŃZ�b�g���Ă�������
/// </summary>
class PostEffectBase : public GameObject {
protected:
	ID3D11ShaderResourceView* _writeSRV = nullptr; // PostEffect�p��SRV

public:
	PostEffectBase() {}
	~PostEffectBase() {}

	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override {}
	virtual void Draw() override {}
	virtual void Draw(ID3D11ShaderResourceView* inputSRV);

	void SetWriteSRV(ID3D11ShaderResourceView* srv) { _writeSRV = srv; }
	ID3D11ShaderResourceView* GetWriteSRV() const { return _writeSRV; }
};
//PostEffectBase.h
//20106_�c���@�@
//25_05_24
#pragma once
#include "GameObject.h"

/// <summary>
/// Draw���]�b�g�̒���SRV���e�X�ŃZ�b�g���Ă�������
/// </summary>
class PostEffectBase : public GameObject {
public:
	PostEffectBase() {}
	~PostEffectBase() {}

	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override {}
	virtual void Draw()override;
};
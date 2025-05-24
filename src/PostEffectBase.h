//PostEffectBase.h
//20106_田中　蓮
//25_05_24
#pragma once
#include "GameObject.h"

/// <summary>
/// Drawメゾットの中でSRVを各々でセットしてください
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
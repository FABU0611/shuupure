//Gaussian.h
//20106_“c’†@˜@
//24_11_07
#pragma once
#include "PostEffectBase.h"

class GaussianX : public PostEffectBase{

public:
	GaussianX() {}
	~GaussianX() {}

	void Draw(ID3D11ShaderResourceView* inputSRV)override;
};
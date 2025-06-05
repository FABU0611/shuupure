//Gaussian.h
//20106_“c’†@˜@
//24_11_07
#include "GaussianY.h"
#include "Renderer.h"
#include "ShaderManager.h"

void GaussianY::Draw(ID3D11ShaderResourceView* inputSRV) {
	Renderer::BeginBlurY();
	_writeSRV = Renderer::GetBYTexture();

	Renderer::SetParameter({ 0.0f, 1.0f, 0.0f, 0.0f });

	Shader::SetShader(ShaderName::Gaussian);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &inputSRV);

	PostEffectBase::Draw(inputSRV);
}
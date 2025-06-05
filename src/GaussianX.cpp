//Gaussian.h
//20106_“c’†@˜@
//24_11_07
#include "GaussianX.h"
#include "Renderer.h"
#include "ShaderManager.h"

void GaussianX::Draw(ID3D11ShaderResourceView* inputSRV) {
	Renderer::BeginBlurX();
	_writeSRV = Renderer::GetBXTexture();

	Renderer::SetParameter({ 1.0f, 0.0f, 0.0f, 0.0f });

	Shader::SetShader(ShaderName::Gaussian);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &inputSRV);

	PostEffectBase::Draw(inputSRV);
}

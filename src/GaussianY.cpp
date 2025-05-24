//Gaussian.h
//20106_“c’†@˜@
//24_11_07
#include "GaussianY.h"
#include "Renderer.h"
#include "ShaderManager.h"

void GaussianY::Draw() {
	Renderer::SetParameter({ 0.0f, 1.0f, 0.0f, 0.0f });

	Shader::SetShader(ShaderName::Gaussian);

	ID3D11ShaderResourceView* bxTexture = Renderer::GetBXTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &bxTexture);

	PostEffectBase::Draw();
}
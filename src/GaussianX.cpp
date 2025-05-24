//Gaussian.h
//20106_“c’†@˜@
//24_11_07
#include "GaussianX.h"
#include "Renderer.h"
#include "ShaderManager.h"

void GaussianX::Draw() {
	Renderer::SetParameter({ 1.0f, 0.0f, 0.0f, 0.0f });

	Shader::SetShader(ShaderName::Gaussian);

	ID3D11ShaderResourceView* mbTexture = Renderer::GetMBTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &mbTexture);

	PostEffectBase::Draw();
}

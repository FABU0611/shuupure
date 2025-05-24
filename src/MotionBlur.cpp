//MotionBlur.h
//20106_“c’†@˜@
//24_11_08
#include "MotionBlur.h"
#include "Renderer.h"
#include "ShaderManager.h"

void MotionBlur::Draw() {
	Shader::SetShader(ShaderName::MotionBlur);

	ID3D11ShaderResourceView* peTexture = Renderer::GetPETexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &peTexture);
	ID3D11ShaderResourceView* velTexture = Renderer::GetVelocityTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &velTexture);

	PostEffectBase::Draw();
}

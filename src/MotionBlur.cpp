//MotionBlur.h
//20106_“c’†@˜@
//24_11_08
#include "MotionBlur.h"
#include "Renderer.h"
#include "ShaderManager.h"

void MotionBlur::Draw(ID3D11ShaderResourceView* inputSRV) {
	Renderer::BeginMotionBlur();
	_writeSRV = Renderer::GetMBTexture();

	Shader::SetShader(ShaderName::MotionBlur);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &inputSRV);
	ID3D11ShaderResourceView* velTexture = Renderer::GetVelocityTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &velTexture);

	PostEffectBase::Draw(inputSRV);
}

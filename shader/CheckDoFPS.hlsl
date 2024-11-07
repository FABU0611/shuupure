//CheckDoFPS.hlsl
//20106_�c���@�@
//24_11_01

#include "common.hlsl"

Texture2D<float> g_DepthTexture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
    //�[�x�l
	float depthValue = g_DepthTexture.Sample(g_SamplerState, In.TexCoord).r;
	
	float blurFactor;
	
	DepthofField(depthValue, blurFactor);
	
	//�[�x�Ɋ�Â����O���[�X�P�[���̏o��    
	Out.Out0 = float4(blurFactor, blurFactor, blurFactor, 1.0f);
	Out.Out0 *= In.Diffuse;
	Out.Out0.a = 1.0f;
}
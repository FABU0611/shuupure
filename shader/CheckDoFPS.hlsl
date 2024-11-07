//CheckDoFPS.hlsl
//20106_田中　蓮
//24_11_01

#include "common.hlsl"

Texture2D<float> g_DepthTexture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
    //深度値
	float depthValue = g_DepthTexture.Sample(g_SamplerState, In.TexCoord).r;
	
	float blurFactor;
	
	DepthofField(depthValue, blurFactor);
	
	//深度に基づいたグレースケールの出力    
	Out.Out0 = float4(blurFactor, blurFactor, blurFactor, 1.0f);
	Out.Out0 *= In.Diffuse;
	Out.Out0.a = 1.0f;
}
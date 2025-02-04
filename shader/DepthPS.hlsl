//DepthPS.hlsl
//20106_田中　蓮
//24_09_28

#include "common.hlsl"

Texture2D g_SceneTexture : register(t0);
Texture2D g_BlurTexture : register(t1);
Texture2D<float> g_DepthTexture : register(t2);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
    //深度値
	float4 scene = g_SceneTexture.Sample(g_SamplerState, In.TexCoord);
	float4 blur = g_BlurTexture.Sample(g_SamplerState, In.TexCoord);
	float depthValue = g_DepthTexture.Sample(g_SamplerState, In.TexCoord).r;
	
	float blurFactor;
	
	DepthofField(depthValue, blurFactor);

	//線形補完でぼかしを計算
	Out.Out0 = lerp(scene, blur, blurFactor);
	Out.Out0 *= In.Diffuse;
	Out.Out0.a = 1.0f;
}
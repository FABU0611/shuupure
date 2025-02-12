//GaussianPS_X.hlsl
//20106_田中　蓮
//24_09_27

#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s1);

void main(in PS_IN In, out PS_OUT Out) {
	//代表点のピクセルの処理
	Out.Out0.a = g_Texture.Sample(g_SamplerState, In.TexCoord).a;
	Out.Out0.rgb = Weights[0].x * g_Texture.Sample(g_SamplerState, In.TexCoord).rgb;
	
	Out.Out0.rgb += Weights[0].y * g_Texture.Sample(g_SamplerState, In.TexCoord + float2( 2.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[0].z * g_Texture.Sample(g_SamplerState, In.TexCoord + float2( 4.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[0].w * g_Texture.Sample(g_SamplerState, In.TexCoord + float2( 6.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].x * g_Texture.Sample(g_SamplerState, In.TexCoord + float2( 8.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].y * g_Texture.Sample(g_SamplerState, In.TexCoord + float2(10.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].z * g_Texture.Sample(g_SamplerState, In.TexCoord + float2(12.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].w * g_Texture.Sample(g_SamplerState, In.TexCoord + float2(14.0f / Screen.x, 0.0f)).rgb;
	
	Out.Out0.rgb += Weights[0].y * g_Texture.Sample(g_SamplerState, In.TexCoord - float2( 2.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[0].z * g_Texture.Sample(g_SamplerState, In.TexCoord - float2( 4.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[0].w * g_Texture.Sample(g_SamplerState, In.TexCoord - float2( 6.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].x * g_Texture.Sample(g_SamplerState, In.TexCoord - float2( 8.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].y * g_Texture.Sample(g_SamplerState, In.TexCoord - float2(10.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].z * g_Texture.Sample(g_SamplerState, In.TexCoord - float2(12.0f / Screen.x, 0.0f)).rgb;
	Out.Out0.rgb += Weights[1].w * g_Texture.Sample(g_SamplerState, In.TexCoord - float2(14.0f / Screen.x, 0.0f)).rgb;
	
	Out.Out0.a = In.Diffuse.a;
}
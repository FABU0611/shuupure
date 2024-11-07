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

	//深度に基づいたグレースケールの出力
	Out.Out0 = lerp(scene, blur, blurFactor);
	Out.Out0 *= In.Diffuse;
	Out.Out0.a = 1.0f;
}
	
 //   //クリップ空間の深度を取得
	//float depth = depthValue;

 //   //深度の線形化
	//float linearDepth = (2.0f * DoF.x) / (DoF.y + DoF.x - depth * (DoF.y - DoF.x));

 //   //0～1の範囲にクランプ
	//linearDepth = clamp(linearDepth, 0.0f, 1.0f);
    
	////outDiffuse = float4(linearDepth, linearDepth, linearDepth, 1.0f);
		
	
	////焦点距離と範囲に基づいて調整
	//float focalDistance = 0.5f; // 焦点距離
	//float focalRange = DoF.z; // ピントが合う範囲

	////グレースケールの深度を焦点距離に基づいて変換
	//float blurFactor = saturate(abs(linearDepth - focalDistance) / focalRange);

	////0?1の範囲にクランプ
	//blurFactor = clamp(blurFactor, 0.0f, 1.0f);
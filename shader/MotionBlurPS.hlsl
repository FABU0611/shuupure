//MotionBlurPS.hlsl
//20106_田中　蓮
//24_11_08

#include "common.hlsl"

Texture2D g_SceneTexture : register(t0);
Texture2D g_VelTexture : register(t1);
SamplerState g_SamplerState : register(s0);

float blurStrength = 10.2f; // ブラー強度

void main(in PS_IN In, out PS_OUT Out) {
    // 通常のシーンカラー取得
	float4 sceneColor = g_SceneTexture.Sample(g_SamplerState, In.TexCoord);
    
    // 速度マップからピクセルの速度ベクトル取得
	float2 velocity = g_VelTexture.Sample(g_SamplerState, In.TexCoord).xy;
    
    // サンプル累積
	float4 blurredColor = sceneColor;
	int sampleCount = 10;
    
    // 速度ベクトルに基づいて複数回サンプリング
	for(int i = 1; i < sampleCount; i++) {
		float factor = i / float(sampleCount);
		float2 samplePos = In.TexCoord + normalize(velocity) * factor * blurStrength;
		float4 sampleColor = g_SceneTexture.Sample(g_SamplerState, samplePos);
		blurredColor += sampleColor;
	}
    
    // 平均化
	blurredColor /= sampleCount;
    
    // 出力カラーを設定
	Out.Out0 = blurredColor;
	Out.Out0.a = 1.0f;
}
/*
    // シーンカラーをサンプル
float4 sceneColor = g_SceneTexture.Sample(g_SamplerState, TexCoord);
    
    // 速度ベクトルを取得
float2 velocity = g_VelocityMap.Sample(g_SamplerState, TexCoord).xy;
    
    // ブラー方向と強さを計算
float2 blurDirection = normalize(velocity) * blurScale;
    
    // ブラー処理
float4 blurredColor = sceneColor;
    for (int i = 1;i < blurSamples; i++){
        float factor = float(i) / float(blurSamples);
        float2 samplePos = TexCoord + blurDirection * factor;
        blurredColor += g_SceneTexture.Sample(g_SamplerState, samplePos);
    }
    
    blurredColor /= float(blurSamples);
    
    return blurredColor;
*/
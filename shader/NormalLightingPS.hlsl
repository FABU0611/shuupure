//NormalLightingPS.hlsl
//20106_田中　蓮
//24_09_11

#include "common.hlsl"

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//物体から光源へのベクトル
    float4 lv = normalize(Light.Direction);
	
	
    float4 normal = normalize(In.Normal);
	
	//光源計算
    float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
	
	//テクスチャのピクセルの処理
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//色に明るさを乗算	
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
	//環境光
    outDiffuse.rgb += Light.Ambient.rgb;
	
	//カメラからピクセルへ向かうベクトル
    float3 eyev;
    EyeVector(In, eyev);
	
	//ハーフベクトルを計算
    float3 halfv = -lv.xyz + eyev;
	//正規化
    halfv = normalize(halfv);
	
	//スペキュラ
    float specular = dot(normal.xyz, halfv);
    specular = saturate(specular);
    specular = pow(specular, 50);

    outDiffuse.rgb += specular;
}
//TangentNormalLightingPS.hlsl
//20106_田中　蓮
//24_09_11

#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Normal : register(t1);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//物体から光源へのベクトル
    float4 lv = normalize(Light.Direction);
	
	//光の減衰
    float ofs = 1.0f;
		
    float4 localnormal = g_Normal.Sample(g_SamplerState, In.TexCoord);
	//RGBをベクトルに
    localnormal = (localnormal * 2.0f) - 1.0f; //-1～1にする
    localnormal.w = 0.0f;
	
    localnormal = normalize(localnormal);
	
	
	//タンジェント
    float4 tangent = normalize(In.Tangent) * Light.Direction.x;
	//バイノーマル
    float4 binormal = normalize(In.Binormal);
	//ノーマル
    float4 normal = normalize(In.Normal);
	
	//接空間を表す行列を作成する
    matrix mat = matrix(tangent, //X軸
						binormal, //Y軸
						normal, //Z軸
						float4(0.0f, 0.0f, 0.0f, 0.0f));
	
	//法線マップ内の法線を接空間へ変換する
    normal = mul(localnormal, mat);
    normal = normalize(normal);
	
	//光源計算
    float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
	//減衰率を乗算
    light *= ofs;
	
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

    outDiffuse.rgb += (specular * ofs);
}
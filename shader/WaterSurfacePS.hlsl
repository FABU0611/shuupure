//WaterSurfacePS.hlsl
//20106_田中　蓮
//24_09_10

#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Normal : register(t1);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
	//物体から光源へのベクトル
	float4 lv = normalize(Light.Direction);
	
	//光の減衰
	float ofs = 1.0f;
	
    //UVオフセットを適切に設定
	float2 uv1 = In.TexCoord;
	float2 uv2 = In.TexCoord;
	float2 uv3 = In.TexCoord;
	
	uv1 = uv1 * 3 + float2(Light.PointParam.y, Light.PointParam.z);
	uv2 = uv2 * 2 + float2(Light.PointParam.y, -Light.PointParam.z);
	uv3 = uv3 * 1 + float2(Light.PointParam.y, 0.0f);
	
	float4 localnormal1 = g_Normal.Sample(g_SamplerState, uv1);
	float4 localnormal2 = g_Normal.Sample(g_SamplerState, uv2);
	float4 localnormal3 = g_Normal.Sample(g_SamplerState, uv3);
	//RGBをベクトルに
	localnormal1 = (localnormal1 * 2.0f) - 1.0f; //-1～1にする
	localnormal2 = (localnormal2 * 2.0f) - 1.0f; //-1～1にする
	localnormal3 = (localnormal3 * 2.0f) - 1.0f; //-1～1にする
	localnormal1.w = 0.0f;
	localnormal2.w = 0.0f;
	localnormal3.w = 0.0f;

	localnormal2 += localnormal1;
	localnormal2 += localnormal3;
	localnormal2 = normalize(localnormal2);
	
	
	//タンジェント
	float4 tangent = normalize(In.Tangent) * -Light.Direction.x;
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
	normal = mul(localnormal2, mat);
	normal = normalize(normal);
	
	//光源計算
	float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
	//減衰率を乗算
	light *= ofs;
	
	//テクスチャのピクセルの処理
	Out.Out0 = float4(0.1f, 0.1f, 1.0f, 0.7f);
	//色に明るさを乗算	
	Out.Out0.rgb *= In.Diffuse.rgb * light;
	Out.Out0.a *= In.Diffuse.a;
	//環境光
	Out.Out0.rgb += Light.Ambient.rgb;
	
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
	specular = pow(specular, 10) * 2;

	Out.Out0.rgb += (specular * ofs);
	
    //フレネル反射
	float vdotn = -dot(eyev, normal.rgb);
	float fresnel = 0.02 + (1.0 - 0.02) * pow(1.0 - vdotn, 500);
	fresnel = min(fresnel, 1) * 0.075f;
	Out.Out0.rgb = lerp(Out.Out0.rgb, float3(1.0f, 1.0f, 1.0f), fresnel);
	
	Out.Out1 = In.Position.z;
	
	float2 velocity = (In.Position.xy / In.Position.w) - (In.PrevPosition.xy / In.PrevPosition.w);
	float2 velocityNormalized = clamp(velocity * 0.5 + 0.5, 0.0, 1.0);
	Out.Out2 = velocityNormalized;
}
//WaterSurfacePS.hlsl
//20106_田中　蓮
//24_09_10

#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Normal : register(t1);
Texture2D g_Env : register(t2);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
	//物体から光源へのベクトル
	float4 lv = normalize(Light.Direction);
		
    //UVオフセットを適切に設定
	float2 uv1 = In.TexCoord;
	float2 uv2 = In.TexCoord;
	float2 uv3 = In.TexCoord;
	
	uv1 = uv1 * 1.5f - float2(Parameter.y, Parameter.z);
	uv2 = uv2 * 1.0f - float2(Parameter.y, -Parameter.z);
	uv3 = uv3 * 0.5f - float2(Parameter.y, 0.0f);
	
	float2 localnormal1 = g_Normal.Sample(g_SamplerState, uv1).rg;
	float2 localnormal2 = g_Normal.Sample(g_SamplerState, uv2).rg;
	float2 localnormal3 = g_Normal.Sample(g_SamplerState, uv3).rg;
	//RGをベクトルに
	localnormal1 = (localnormal1 * 2.0f) - 1.0f; //-1～1にする
	localnormal2 = (localnormal2 * 2.0f) - 1.0f; //-1～1にする
	localnormal3 = (localnormal3 * 2.0f) - 1.0f; //-1～1にする
	
	//Bチャンネルを再計算
	float4 normal1 = float4(localnormal1.r, localnormal1.g, sqrt(saturate(1.0f - dot(localnormal1.r, localnormal1.g))), 0.0f);
	float4 normal2 = float4(localnormal2.r, localnormal2.g, sqrt(saturate(1.0f - dot(localnormal2.r, localnormal2.g))), 0.0f);
	float4 normal3 = float4(localnormal3.r, localnormal3.g, sqrt(saturate(1.0f - dot(localnormal3.r, localnormal3.g))), 0.0f);
		
	normal2 += normal1;
	normal2 += normal3;
	normal2 = normalize(normal2);
	
	
	//タンジェント
	float4 tangent = normalize(In.Tangent);
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
	normal = mul(normal2, mat);
	normal = normalize(normal);
	
	//光源計算
	float light = 0.5f - 0.5f * dot(lv.xyz, normal.xyz);
	
	//テクスチャのピクセルの処理
	Out.Out0 = float4(0.2f, 0.2f, 0.5f, 1.0f);
	//色に明るさを乗算	
	Out.Out0.rgb *= In.Diffuse.rgb * light;
	Out.Out0.a *= In.Diffuse.a;
	//環境光
	Out.Out0.rgb += Light.Ambient.rgb;
	
	//カメラからピクセルへ向かうベクトル
	float3 eyev;
	EyeVector(In, eyev);
	
	//ハーフベクトルを計算
	float3 halfv = lv.xyz + eyev;
	//正規化
	halfv = normalize(halfv);
	
	//スペキュラ
	float specular = -dot(halfv, normal.xyz);
	specular = saturate(specular);
	specular = pow(specular, 10) * 2;

	Out.Out0.rgb += specular;
	
    //環境マップ
	float3 refv = reflect(eyev.xyz, normal.xyz);
	refv = normalize(refv);
	
	float2 uv;
	uv.x = -atan2(refv.x, refv.z) / (PI * 2);
	uv.y = acos(refv.y) / PI;
	
	float3 reflect = g_Env.Sample(g_SamplerState, uv);
	
    //フレネル反射
	float vdotn = saturate(dot(-eyev, normal.rgb));
	float fresnel = 0.02f + (1.0f - 0.02f) * pow(1.0f - vdotn, 5);
	fresnel = min(fresnel, 1) * 1.0f;
	Out.Out0.rgb = lerp(Out.Out0.rgb, reflect.rgb, fresnel);
	Out.Out0.a *= fresnel * 1.0f;
		
	CreateVelTex(In, Out.Out1);
}
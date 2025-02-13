//NormalLightingPS.hlsl
//20106_田中　蓮
//24_09_11

#include "common.hlsl"

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
	//物体から光源へのベクトル
	float4 lv = normalize(Light.Direction);
	
	
	float4 normal = normalize(In.Normal);
	
	//光源計算
	float light = 0.5f - 0.5f * dot(lv.xyz, normal.xyz);
	
	//テクスチャのピクセルの処理
	if(Material.TextureEnable) {
		Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
		//色に明るさを乗算
		Out.Out0.rgb *= In.Diffuse.rgb * light;
		Out.Out0.a *= In.Diffuse.a;
	}
	else {
		//色に明るさを乗算
		Out.Out0.rgb = In.Diffuse.rgb * light;
		Out.Out0.a = In.Diffuse.a;
	}
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
	specular = pow(specular, 50);

	Out.Out0.rgb += specular;
	
	CreateVelTex(In, Out.Out1);
}
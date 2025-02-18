//DepthShadowPS.hlsl
//20106_田中　蓮
//24_12_02
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Normal : register(t1);
Texture2D g_Shadow0 : register(t2);
Texture2D g_Shadow1 : register(t3);
Texture2D g_Shadow2 : register(t4);
Texture2D g_Shadow3 : register(t5);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerBorder : register(s2);


void main(in PS_IN In, out PS_OUT Out) {
	float4 lv = normalize(Light.Direction);
	
	float3 eyev;
	EyeVector(In, eyev);
	float distance = length(eyev);
		
	//ノーマル
	float4 normal = normalize(In.Normal);
	if(Material.NormalTextureEnable) {
		float4 localnormal = g_Normal.Sample(g_SamplerState, In.TexCoord);
		//RGBをベクトルに
		localnormal = (localnormal * 2.0f) - 1.0f; //-1～1にする
		localnormal.w = 0.0f;
		
		float scale = 1.0f / (1.0f + distance * 0.1f);
	
		localnormal = normalize(localnormal);
	
	
		//タンジェント
		float4 tangent = normalize(In.Tangent);
		//バイノーマル
		float4 binormal = normalize(In.Binormal);
	
		//接空間を表す行列を作成する
		matrix mat = matrix(tangent, //X軸
						binormal, //Y軸
						normal, //Z軸
						float4(0.0f, 0.0f, 0.0f, 0.0f));
	
		//法線マップ内の法線を接空間へ変換する
		normal = mul(localnormal, mat);
	}
	normal = normalize(normal);
	
	//光源計算
	float light = 0.5f - 0.5f * dot(lv.xyz, normal.xyz);
	
	if(Material.TextureEnable) {
		Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
		Out.Out0.rgb *= In.Diffuse.rgb;
		Out.Out0.a *= In.Diffuse.a;
	}
	else {
		Out.Out0.rgb = In.Diffuse.rgb;
		Out.Out0.a = In.Diffuse.a;
	}
	Out.Out0.rgb *= light;
	Out.Out0.rgb += Light.Ambient.rgb;
	
	float4 specular = 0.0f;
	if(Material.Shininess > 0.0f) { //スペキュラ
		//ハーフベクトルを計算
		float3 halfv = lv.xyz + eyev;
		halfv = normalize(halfv);
	
		//スペキュラ
		float HdotN = -dot(halfv, normal.xyz);
		HdotN = saturate(HdotN);
		float3 specularpow = pow(HdotN, Material.Shininess);
		specular = float4(specularpow, 1.0f) * Material.Specular;
	}
	
	{	//影
		//カスケードの決定
		int cascadeidx = 3;
		for(int i = 0; i < 4; i++) {
			float zLVP = In.LightPosition[i].z / In.LightPosition[i].w;
			if(zLVP >= 0.0f && zLVP <= 1.0f) {
			
				float2 lightUV = In.LightPosition[i].xy / In.LightPosition[i].w;
				lightUV.x = lightUV.x * 0.5f + 0.5f;
				lightUV.y = lightUV.y * -0.5f + 0.5f;
		
				float lightdepth = 0.0f;
				if(lightUV.x >= 0.01f && lightUV.x <= 0.999f &&
				lightUV.y >= 0.001f && lightUV.y <= 0.899f) {
					cascadeidx = i;
	
					float size[4] = { 2048.0f, 1024.0f, 512.0f, 256.0f };
					float offset = 1.0f / size[i];
					int samplecount = 1;
	
					for(int x = -samplecount; x <= samplecount; x++) {
						for(int y = -samplecount; y <= samplecount; y++) {
							float sampledepth = 0.0f;
						//深度バッファからこのピクセルのライトから距離を取得	
							if(i == 0) {
								sampledepth = g_Shadow0.Sample(g_SamplerBorder, lightUV.xy + float2(x, y) * offset).r;
							}
							else if(i == 1) {
								sampledepth = g_Shadow1.Sample(g_SamplerBorder, lightUV.xy + float2(x, y) * offset).r;
							}
							else if(i == 2) {
								sampledepth = g_Shadow2.Sample(g_SamplerBorder, lightUV.xy + float2(x, y) * offset).r;
							}
							else if(i == 3) {
								sampledepth = 1.0f;
							}
							if(zLVP <= sampledepth + 0.005f) {
								lightdepth += 1.0f;
								Out.Out0.rgb += specular.rgb;
							}
						}
					}
					lightdepth /= ((2.0f * samplecount + 1.0f) * (2.0f * samplecount + 1.0f));

					float3 color = Out.Out0.rgb;
					float3 shadowcolor = Out.Out0 * 0.5f;
					Out.Out0.rgb = lerp(shadowcolor, color, lightdepth);
					break;
			
				}
			}
		}
		
		//カスケード色分け切り替え
		if(Parameter.x == 1.0f) {
			switch(cascadeidx) {
				case 0:
					Out.Out0.rgb += float3(0.3f, 0.0f, 0.0f);
					break;
		
				case 1:
					Out.Out0.rgb += float3(0.0f, 0.3f, 0.0f);
					break;
		
				case 2:
					Out.Out0.rgb += float3(0.0f, 0.0f, 0.3f);
					break;
				
				case 3:
					Out.Out0.rgb += float3(0.3f, 0.3f, 0.0f);
					break;
			}
		}
	}
	
	
	Out.Out0.a = 1.0f;
}
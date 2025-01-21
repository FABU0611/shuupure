//DepthShadowPS.hlsl
//20106_田中　蓮
//24_12_02
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Shadow0 : register(t2);
Texture2D g_Shadow1 : register(t3);
Texture2D g_Shadow2 : register(t4);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerBorder : register(s2);

void main(in PS_IN In, out PS_OUT Out) {
	Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
	Out.Out0 *= In.Diffuse;
	
	/*
	for(int i = 0; i < 3; i++) {
		//ライトビュースクリーン空間でのZ値を計算する
		float zInLVP = In.LightPosition[i].z / In.LightPosition[i].w;
		if(zInLVP >= 0.0f && zInLVP <= 1.0f) {
			//Z値を見て、このピクセルがこのシャドウに含まれているかを判定
			float2 shadowUV = In.LightPosition[i].xy / In.LightPosition[i].w;
			shadowUV *= float2(0.5f, -0.5f);
			shadowUV += 0.5f;
			
			//シャドウUVが範囲内か判定
			if(shadowUV.x >= 0.0f && shadowUV.x <= 1.0f &&
				shadowUV.y >= 0.0f && shadowUV.y <= 1.0f) {
				float lightdepth = 0.0f;
				if(i == 0) {
					lightdepth = g_Shadow0.Sample(g_SamplerBorder, shadowUV.xy).r;
				}
				else if(i == 1) {
					lightdepth = g_Shadow1.Sample(g_SamplerBorder, shadowUV.xy).r;
				}
				else if(i == 2) {
					lightdepth = g_Shadow2.Sample(g_SamplerBorder, shadowUV.xy).r;
				}
				switch(i) {
					case 0:
						Out.Out0.rgb = float3(0.3f, 0.0f, 0.0f); // インデックスを色で表示
						break;
	
					case 1:
						Out.Out0.rgb = float3(0.0f, 0.3f, 0.0f); // インデックスを色で表示
						break;
	
					case 2:
						Out.Out0.rgb = float3(0.0f, 0.0f, 0.3f); // インデックスを色で表示
						break;
				}
				if(zInLVP - 0.005f >= lightdepth) {
					Out.Out0.rgb *= 0.5f;
					break;
				}
			}

		}
	}
	*/
	
	
	//カスケードの決定
	int cascadeidx = 2;
	for(int i = 0; i < 3; i++) {
		float4 lightpos = In.LightPosition[i];
		lightpos.xy /= lightpos.w;
		lightpos.x = lightpos.x * 0.5f + 0.5f;
		lightpos.y = -lightpos.y * 0.5f + 0.5f;
		
		if((In.Position.z <= ((Split[i] - 1.0f) / (Split[2] - 1.0f))) &&
			(lightpos.x >= 0.0f && lightpos.x <= 1.0f &&
			lightpos.y >= 0.0f && lightpos.y <= 1.0f)) {
			cascadeidx = i;
			break;
		}
	}
	
	
	//cascadeidx = 0;

		
	//画面上での座標をピクセル座標にする
	float4 lightpos = In.LightPosition[cascadeidx];
	lightpos.xyz /= lightpos.w;
	lightpos.x = lightpos.x * 0.5f + 0.5f;
	lightpos.y = -lightpos.y * 0.5f + 0.5f;
	
	float lightdepth = 0.0f;
	if(lightpos.x >= 0.0f && lightpos.x <= 1.0f &&
		lightpos.y >= 0.0f && lightpos.y <= 1.0f) {
	
		//深度バッファからこのピクセルのライトから距離を取得	
		if(cascadeidx == 0) {
			lightdepth = g_Shadow0.Sample(g_SamplerBorder, lightpos.xy).r;
		}
		else if(cascadeidx == 1) {
			lightdepth = g_Shadow1.Sample(g_SamplerBorder, lightpos.xy).r;
		}
		else if(cascadeidx == 2) {
			lightdepth = g_Shadow2.Sample(g_SamplerBorder, lightpos.xy).r;
		}
	}
	
	if(lightpos.z - 0.005f > lightdepth) {
		Out.Out0.rgb *= 0.5f;
	}

	//Out.Out0.rgb = float3(lightdepth, lightdepth, lightdepth);
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
	}
	
	
	
	/*
	int cascadeidx = 0;
	float maxDepth = -1.0f;
	float z[4] = { 1.0f, 59.0f, 134.0f, 330.0f };
	
	for(int i = 0; i < 3; i++) {
		float zInLVP = In.LightPosition[i].z;
		float near = ((z[i] - 1.0f) / (z[3] - 1.0f));
		float far = ((z[i + 1] - 1.0f) / (z[3] - 1.0f));
		if(zInLVP >= 0.0f && zInLVP <= 1.0f) {
			float4 lightpos = In.LightPosition[i];
			lightpos.z /= lightpos.w;
			
			if(zInLVP > maxDepth) {
				cascadeidx = i;
				maxDepth = lightpos.z;
				break;
			}
		}
	}
//	cascadeidx = 2;
	float4 lightpos = In.LightPosition[cascadeidx];
	lightpos.xyz /= lightpos.w;
	lightpos.x = lightpos.x * 0.5f + 0.5f;
	lightpos.y = -lightpos.y * 0.5f + 0.5f;
	
	float lightdepth = 0.0f;
	if(lightpos.x >= 0.0f && lightpos.x <= 1.0f &&
		lightpos.y >= 0.0f && lightpos.y <= 1.0f) {
	
		//深度バッファからこのピクセルのライトから距離を取得	
		if(cascadeidx == 0) {
			lightdepth = g_Shadow0.Sample(g_SamplerBorder, lightpos.xy).r;
		}
		else if(cascadeidx == 1) {
			lightdepth = g_Shadow1.Sample(g_SamplerBorder, lightpos.xy).r;
		}
		else if(cascadeidx == 2) {
			lightdepth = g_Shadow2.Sample(g_SamplerBorder, lightpos.xy).r;
		}
	}
	if(lightdepth == 1.0f) {
		lightdepth = 0.0f;
	}
	
	//影をかける
	if(lightpos.z - 0.005f > lightdepth) {
		Out.Out0.rgb *= 0.5f;
	}
	*/
	
//	Out.Out0.rgb = float3(lightpos.rg, 0.0f);
//	Out.Out0.rgb = float3(lightdepth, lightdepth, lightdepth);
	//カスケードを色で表示
	//switch(cascadeidx) {
	//	case 0:
	//		Out.Out0.rgb = float3(0.3f, 0.0f, 0.0f);
	//		break;
	
	//	case 1:
	//		Out.Out0.rgb = float3(0.0f, 0.3f, 0.0f);
	//		break;
	
	//	case 2:
	//		Out.Out0.rgb = float3(0.0f, 0.0f, 0.3f);
	//		break;
	//}
	
	
	Out.Out0.a = 1.0f;
}
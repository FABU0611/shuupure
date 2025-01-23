//DepthShadowPS.hlsl
//20106_田中　蓮
//24_12_02
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Shadow0 : register(t2);
Texture2D g_Shadow1 : register(t3);
Texture2D g_Shadow2 : register(t4);
Texture2D g_Shadow3 : register(t5);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerBorder : register(s2);

void main(in PS_IN In, out PS_OUT Out) {
	Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
	Out.Out0 *= In.Diffuse;
		
	
	//カスケードの決定
	int cascadeidx = 3;
	for(int i = 0; i < 4; i++) {
		float4 lightpos = In.LightPosition[i];
		lightpos.xy /= lightpos.w;
		lightpos.x = lightpos.x * 0.5f + 0.5f;
		lightpos.y = -lightpos.y * 0.5f + 0.5f;
		
		if((lightpos.x >= 0.00001f && lightpos.x <= 0.99999f &&
			lightpos.y >= 0.00001f && lightpos.y <= 0.99999f)) {
			cascadeidx = i;
			break;
		}
	}
	
		
	//画面上での座標をピクセル座標にする
	float4 lightpos = In.LightPosition[cascadeidx];
	lightpos.xyz /= lightpos.w;
	lightpos.x = lightpos.x * 0.5f + 0.5f;
	lightpos.y = -lightpos.y * 0.5f + 0.5f;
	
	float lightdepth = 0.0f;
	float offset = 1.0f / 1024.0f;
	int samplecount = 2;
	
	if(lightpos.x >= 0.0f  && lightpos.x <= 1.0f  &&
		lightpos.y >= 0.0f  && lightpos.y <= 1.0f ) { //境界線が見えてしまうのでマップの下の部分は1.0までサンプルしない
	
		for(int x = -samplecount; x <= samplecount; x++) {
			for(int y = -samplecount; y <= samplecount; y++) {
				float sampledepth = 0.0f;
				//深度バッファからこのピクセルのライトから距離を取得	
				if(cascadeidx == 0) {
					sampledepth = g_Shadow0.Sample(g_SamplerBorder, lightpos.xy + float2(x, y) * offset).r;
					lightdepth += (lightpos.z <= sampledepth + 0.005f) ? 1.0f : 0.0f;
				}
				else if(cascadeidx == 1) {
					sampledepth = g_Shadow1.Sample(g_SamplerBorder, lightpos.xy + float2(x, y) * offset).r;
					lightdepth += (lightpos.z <= sampledepth + 0.005f) ? 1.0f : 0.0f;
				}
				else if(cascadeidx == 2) {
					sampledepth = g_Shadow2.Sample(g_SamplerBorder, lightpos.xy + float2(x, y) * offset).r;
					lightdepth += (lightpos.z <= sampledepth + 0.005f) ? 1.0f : 0.0f;
				}
				else if(cascadeidx == 3) {
					sampledepth = g_Shadow3.Sample(g_SamplerBorder, lightpos.xy + float2(x, y) * offset).r;
					lightdepth += (lightpos.z <= sampledepth + 0.005f) ? 1.0f : 0.0f;
				}
			}
		}
		lightdepth /= ((2.0f * samplecount + 1.0f) * (2.0f * samplecount + 1.0f));
	}
	else {
			lightdepth = 0.0f;
	}


	float3 color = Out.Out0.rgb;
	float3 shadowcolor = Out.Out0 * 0.5f;
	Out.Out0.rgb = lerp(shadowcolor, color, lightdepth);

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
			
		case 3:
			Out.Out0.rgb += float3(0.3f, 0.3f, 0.0f);
			break;
	}
	
	
	Out.Out0.a = 1.0f;
}
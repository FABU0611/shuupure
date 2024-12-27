//DepthShadowPS.hlsl
//20106_田中　蓮
//24_12_02
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Shadow : register(t2);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerState1 : register(s1);

void main(in PS_IN In, out PS_OUT Out) {
	Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
	Out.Out0 *= In.Diffuse;
	
	//Light.Positionはライトから見たピクセルの3D空間での座標
	//画面内での座標に変換
	In.LightPosition.xyz /= In.LightPosition.w;
	//画面上での座標をピクセル座標にする
	In.LightPosition.x = In.LightPosition.x * 0.5f + 0.5f;
	In.LightPosition.y = -In.LightPosition.y * 0.5f + 0.5f;
		
	//深度バッファからこのピクセルのライトから距離を取得	
	float lightdepth = g_Shadow.Sample(g_SamplerState1, In.LightPosition.xy).r;
	
	if(In.LightPosition.z - 0.005f > lightdepth) {
		Out.Out0.rgb *= 0.5f;
	}
	
}
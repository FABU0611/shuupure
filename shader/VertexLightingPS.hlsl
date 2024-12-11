//VertexLightingPS.hlsl
//20106_ìcíÜÅ@ò@
//24_12_09
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target) {

	if(Material.TextureEnable) {
		outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
		outDiffuse *= In.Diffuse;
	}
	else {
		outDiffuse = In.Diffuse;
	}
	outDiffuse.a = 1.0f;
}


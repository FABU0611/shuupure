//unlitTextureVS.hlsl
//20106_ìcíÜÅ@ò@
//24_04_24
#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out PS_OUT Out) {

	if(Material.TextureEnable) {
		Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
		Out.Out0 *= In.Diffuse;
	}
	else {
		Out.Out0 = In.Diffuse;
	}
	Out.Out0.a = In.Diffuse.a;
	
	Out.Out1 = In.Position.z;
	
	float2 velocity = (In.Position.xy / In.Position.w) - (In.PrevPosition.xy / In.PrevPosition.w);
	float2 velocityNormalized = clamp(velocity * 0.5 + 0.5, 0.0, 1.0);
	Out.Out2 = velocityNormalized;
}

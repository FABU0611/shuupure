//NormalLightingVS.hlsl
//20106_�c���@�@
//24_09_11

#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out) {
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);
    
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
    
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.TexCoord = In.TexCoord;
    
	Out.WorldPosition = mul(In.Position, World);
}
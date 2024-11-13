//NormalLightingVS.hlsl
//20106_ìcíÜÅ@ò@
//24_09_11

#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out) {
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	float4 curPos = mul(In.Position, wvp);
	Out.Position = curPos;
    
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
    
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.TexCoord = In.TexCoord;
    
	Out.WorldPosition = mul(In.Position, World);

	matrix prevmvp;
	prevmvp = mul(PrevWorld, PrevView);
	prevmvp = mul(prevmvp, PrevProjection);
	float4 prevPos = mul(In.Position, prevmvp);
	
	Out.Velocity = (curPos.xy / curPos.w) - (prevPos.xy / prevPos.w);
}
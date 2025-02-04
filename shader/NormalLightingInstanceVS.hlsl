//NormalLightingVS.hlsl
//20106_田中　蓮
//24_09_11

#include "common.hlsl"

StructuredBuffer<float3> g_position : register(t2);

void main(in VS_IN In, out PS_IN Out) {
	//今フレームのインスタンスごとの頂点座標を計算
	Out.CurPosition = mul(In.Position, World);
	Out.CurPosition.xyz += g_position[In.InstanceID].xyz;
	
	matrix lightwvp;
	for(int i = 0; i < 4; i++) {
		Out.LightPosition[i] = mul(In.Position, World);
		Out.LightPosition[i].xyz += g_position[In.InstanceID].xyz;
		Out.LightPosition[i] = mul(Out.LightPosition[i], Light.ViewMatrix[i]);
		Out.LightPosition[i] = mul(Out.LightPosition[i], Light.ProjectionMatrix[i]);
	}
	
	Out.CurPosition = mul(Out.CurPosition, View);
	Out.CurPosition = mul(Out.CurPosition, Projection);
	
	Out.Position = Out.CurPosition;
    
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
    
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.TexCoord = In.TexCoord;
    
	Out.WorldPosition = mul(In.Position, World);

	
	//前フレームのインスタンスごとの頂点座標を計算
	Out.PrevPosition = mul(In.Position, PrevWorld);	
	Out.PrevPosition.xyz += g_position[In.InstanceID].xyz;	
	Out.PrevPosition = mul(Out.PrevPosition, PrevView);
	Out.PrevPosition = mul(Out.PrevPosition, PrevProjection);
}
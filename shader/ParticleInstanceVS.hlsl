
#include "common.hlsl"

struct InstanceOutput {
	float3 pos;
	float dummy;
	float3 scale;
	float dummy2;
	float4 color;
};

StructuredBuffer<InstanceOutput> g_instance : register(t2);


void main(in VS_IN In, out PS_IN Out) {
	//今フレームのインスタンスごとの頂点座標を計算
	In.Position.xyz *= g_instance[In.InstanceID].scale.xyz;
	Out.Position = mul(In.Position, World);
	Out.Position.xyz += g_instance[In.InstanceID].pos.xyz;
	Out.Position = mul(Out.Position, View);
	Out.Position = mul(Out.Position, Projection);
    
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;
    
	Out.Diffuse = In.Diffuse * g_instance[In.InstanceID].color;
	Out.TexCoord = In.TexCoord;
    
	Out.WorldPosition = mul(In.Position, World);
}

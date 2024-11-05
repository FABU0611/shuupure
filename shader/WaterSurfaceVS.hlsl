//WaterSurfaceVS.hlsl
//20106_田中　蓮
//24_09_10

#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
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
    
	//タンジェントをワールド変換
    float4 worldTangent, tangent;
    tangent = float4(In.Tangent.xyz, 0.0f);
    worldTangent = mul(tangent, World);
    worldTangent = normalize(worldTangent);
    Out.Tangent = worldTangent;

	//バイノーマル
    float4 worldBinormal;
    worldBinormal.w = 0.0f;
    worldBinormal.xyz = cross(worldTangent.xyz, worldNormal.xyz);
    worldBinormal = normalize(worldBinormal);
    Out.Binormal = worldBinormal;
}
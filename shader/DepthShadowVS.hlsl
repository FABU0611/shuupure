//DepthShadowVS.hlsl
//20106_田中　蓮
//24_12_02
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out){
	//一回目の描画ではライトカメラの行列、二回目の描画では通常カメラの描画
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	
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
	
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.Diffuse.a = In.Diffuse.a;
	
	Out.Position = mul(In.Position, wvp);
	Out.Normal = worldNormal;
	Out.TexCoord = In.TexCoord;
	Out.WorldPosition = mul(In.Position, World);
	
	//ライトカメラで頂点変換して出力
	matrix lightwvp;
	for(int i = 0; i < 4; i++) {
		lightwvp = mul(World, Light.ViewMatrix[i]);
		lightwvp = mul(lightwvp, Light.ProjectionMatrix[i]);
		Out.LightPosition[i] = mul(In.Position, lightwvp);
	}
}
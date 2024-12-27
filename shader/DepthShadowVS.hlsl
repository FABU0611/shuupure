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
	
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.Diffuse.a = In.Diffuse.a;
	
	Out.Position = mul(In.Position, wvp);
	Out.Normal = worldNormal;
	Out.TexCoord = In.TexCoord;
	
	//ライトカメラで頂点変換して出力
	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);
	lightwvp = mul(lightwvp, Light.ProjectionMatrix);
	Out.LightPosition = mul(In.Position, lightwvp);

}
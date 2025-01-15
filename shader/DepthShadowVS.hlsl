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
	
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.Diffuse.a = In.Diffuse.a;
	
	Out.Position = mul(In.Position, wvp);
	Out.Normal = worldNormal;
	Out.TexCoord = In.TexCoord;
	
	//ライトカメラで頂点変換して出力
	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);
	for(int i = 0; i < 3; i++) {
		lightwvp = mul(lightwvp, Light.ProjectionMatrix[i]);
		if(i == 0) {
			Out.LightPosition0 = mul(In.Position, lightwvp);
		}
		else if(i == 1) {
			Out.LightPosition1 = mul(In.Position, lightwvp);
		}
		else if(i == 2) {
			Out.LightPosition2 = mul(In.Position, lightwvp);
		}
	}
}
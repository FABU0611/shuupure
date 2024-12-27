//DepthShadowVS.hlsl
//20106_�c���@�@
//24_12_02
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out){
	//���ڂ̕`��ł̓��C�g�J�����̍s��A���ڂ̕`��ł͒ʏ�J�����̕`��
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
	
	//���C�g�J�����Œ��_�ϊ����ďo��
	matrix lightwvp;
	lightwvp = mul(World, Light.ViewMatrix);
	lightwvp = mul(lightwvp, Light.ProjectionMatrix);
	Out.LightPosition = mul(In.Position, lightwvp);

}
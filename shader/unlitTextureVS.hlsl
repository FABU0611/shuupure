//unlitTextureVS.hlsl
//20106_ìcíÜÅ@ò@
//24_04_24
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out) {

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	float4 curPos = mul(In.Position, wvp);
	Out.Position = curPos;
	
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse * Material.Diffuse;

	matrix prevmvp;
	prevmvp = mul(PrevWorld, PrevView);
	prevmvp = mul(prevmvp, PrevProjection);
	float4 prevPos = mul(In.Position, prevmvp);
	
	Out.Velocity = (curPos.xy / curPos.w) - (prevPos.xy / prevPos.w);
	//Out.Velocity = (Out.Velocity + 1.0f) * 0.5f;
}


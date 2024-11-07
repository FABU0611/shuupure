//unlitTextureVS.hlsl
//20106_ìcíÜÅ@ò@
//24_04_24
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out) {

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse * Material.Diffuse;

}


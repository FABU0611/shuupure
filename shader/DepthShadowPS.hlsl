//DepthShadowPS.hlsl
//20106_�c���@�@
//24_12_02
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Shadow0 : register(t2);
Texture2D g_Shadow1 : register(t3);
Texture2D g_Shadow2 : register(t4);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerBorder : register(s2);

void main(in PS_IN In, out PS_OUT Out) {
	Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
	Out.Out0 *= In.Diffuse;
	
	//�J�X�P�[�h�̌���
	int cascadeidx = 0;
	for(int i = 0; i < 2; i++) {
		if(In.Position.z < ((Split[i] - 1.0f) / (250.0f - 1.0f))) {
			cascadeidx = i;
			break;
		}
	}
	
//	cascadeidx = 1;

		
	//��ʏ�ł̍��W���s�N�Z�����W�ɂ���
	float4 lightpos = In.LightPosition[cascadeidx];
	lightpos.xyz /= lightpos.w;
	lightpos.x = lightpos.x * 0.5f + 0.5f;
	lightpos.y = -lightpos.y * 0.5f + 0.5f;
	
	clamp(lightpos.xy, 0.0f, 1.0f);
	
	//�[�x�o�b�t�@���炱�̃s�N�Z���̃��C�g���狗�����擾	
	float lightdepth = 0.0f;
	if(cascadeidx == 0) {		
		lightdepth = g_Shadow0.Sample(g_SamplerBorder, lightpos.xy).r;
	}
	else if(cascadeidx == 1) {
		lightdepth = g_Shadow1.Sample(g_SamplerBorder, lightpos.xy).r;
	}
	//else if(cascadeidx == 2) {
	//	lightdepth = g_Shadow2.Sample(g_SamplerBorder, lightpos.xy).r;
	//}
	
	if(lightpos.z - 0.005f > lightdepth) {
		Out.Out0.rgb *= 0.5f;
	}
//	Out.Out0.rgb = float3(lightpos.rg, 0.0f);
//	Out.Out0.rgb = float3(lightdepth, lightdepth, lightdepth);
//	Out.Out0.rgb = float3((float(cascadeidx) + 1.0f) / 3.0f, 0.0f, 0.0f); // �C���f�b�N�X��F�ŕ\��

}
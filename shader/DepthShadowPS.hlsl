//DepthShadowPS.hlsl
//20106_�c���@�@
//24_12_02
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Shadow : register(t2);
SamplerState g_SamplerState : register(s0);
SamplerState g_SamplerState1 : register(s1);

void main(in PS_IN In, out PS_OUT Out) {
	Out.Out0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
	Out.Out0 *= In.Diffuse;
	
	//Light.Position�̓��C�g���猩���s�N�Z����3D��Ԃł̍��W
	//��ʓ��ł̍��W�ɕϊ�
	In.LightPosition.xyz /= In.LightPosition.w;
	//��ʏ�ł̍��W���s�N�Z�����W�ɂ���
	In.LightPosition.x = In.LightPosition.x * 0.5f + 0.5f;
	In.LightPosition.y = -In.LightPosition.y * 0.5f + 0.5f;
		
	//�[�x�o�b�t�@���炱�̃s�N�Z���̃��C�g���狗�����擾	
	float lightdepth = g_Shadow.Sample(g_SamplerState1, In.LightPosition.xy).r;
	
	if(In.LightPosition.z - 0.005f > lightdepth) {
		Out.Out0.rgb *= 0.5f;
	}
	
}
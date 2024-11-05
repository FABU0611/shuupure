//NormalLightingPS.hlsl
//20106_�c���@�@
//24_09_11

#include "common.hlsl"

Texture2D g_Texture : register(t0);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//���̂�������ւ̃x�N�g��
    float4 lv = normalize(Light.Direction);
	
	
    float4 normal = normalize(In.Normal);
	
	//�����v�Z
    float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
	
	//�e�N�X�`���̃s�N�Z���̏���
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//�F�ɖ��邳����Z	
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;
	//����
    outDiffuse.rgb += Light.Ambient.rgb;
	
	//�J��������s�N�Z���֌������x�N�g��
    float3 eyev;
    EyeVector(In, eyev);
	
	//�n�[�t�x�N�g�����v�Z
    float3 halfv = -lv.xyz + eyev;
	//���K��
    halfv = normalize(halfv);
	
	//�X�y�L����
    float specular = dot(normal.xyz, halfv);
    specular = saturate(specular);
    specular = pow(specular, 50);

    outDiffuse.rgb += specular;
}
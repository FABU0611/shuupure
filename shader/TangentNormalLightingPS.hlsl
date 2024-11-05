//TangentNormalLightingPS.hlsl
//20106_�c���@�@
//24_09_11

#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Normal : register(t1);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//���̂�������ւ̃x�N�g��
    float4 lv = normalize(Light.Direction);
	
	//���̌���
    float ofs = 1.0f;
		
    float4 localnormal = g_Normal.Sample(g_SamplerState, In.TexCoord);
	//RGB���x�N�g����
    localnormal = (localnormal * 2.0f) - 1.0f; //-1�`1�ɂ���
    localnormal.w = 0.0f;
	
    localnormal = normalize(localnormal);
	
	
	//�^���W�F���g
    float4 tangent = normalize(In.Tangent) * Light.Direction.x;
	//�o�C�m�[�}��
    float4 binormal = normalize(In.Binormal);
	//�m�[�}��
    float4 normal = normalize(In.Normal);
	
	//�ڋ�Ԃ�\���s����쐬����
    matrix mat = matrix(tangent, //X��
						binormal, //Y��
						normal, //Z��
						float4(0.0f, 0.0f, 0.0f, 0.0f));
	
	//�@���}�b�v���̖@����ڋ�Ԃ֕ϊ�����
    normal = mul(localnormal, mat);
    normal = normalize(normal);
	
	//�����v�Z
    float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
	//����������Z
    light *= ofs;
	
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

    outDiffuse.rgb += (specular * ofs);
}
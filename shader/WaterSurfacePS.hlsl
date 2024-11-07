//WaterSurfacePS.hlsl
//20106_�c���@�@
//24_09_10

#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Normal : register(t1);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
	//���̂�������ւ̃x�N�g��
	float4 lv = normalize(Light.Direction);
	
	//���̌���
	float ofs = 1.0f;
	
    //UV�I�t�Z�b�g��K�؂ɐݒ�
	float2 uv1 = In.TexCoord;
	float2 uv2 = In.TexCoord;
	float2 uv3 = In.TexCoord;
	
	uv1 = uv1 * 3 + float2(Light.PointParam.y, Light.PointParam.z);
	uv2 = uv2 * 2 + float2(Light.PointParam.y, -Light.PointParam.z);
	uv3 = uv3 * 1 + float2(Light.PointParam.y, 0.0f);
	
	float4 localnormal1 = g_Normal.Sample(g_SamplerState, uv1);
	float4 localnormal2 = g_Normal.Sample(g_SamplerState, uv2);
	float4 localnormal3 = g_Normal.Sample(g_SamplerState, uv3);
	//RGB���x�N�g����
	localnormal1 = (localnormal1 * 2.0f) - 1.0f; //-1�`1�ɂ���
	localnormal2 = (localnormal2 * 2.0f) - 1.0f; //-1�`1�ɂ���
	localnormal3 = (localnormal3 * 2.0f) - 1.0f; //-1�`1�ɂ���
	localnormal1.w = 0.0f;
	localnormal2.w = 0.0f;
	localnormal3.w = 0.0f;

	localnormal2 += localnormal1;
	localnormal2 += localnormal3;
	localnormal2 = normalize(localnormal2);
	
	
	//�^���W�F���g
	float4 tangent = normalize(In.Tangent) * -Light.Direction.x;
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
	normal = mul(localnormal2, mat);
	normal = normalize(normal);
	
	//�����v�Z
	float light = 0.5f - 0.5f * dot(normal.xyz, lv.xyz);
	//����������Z
	light *= ofs;
	
	//�e�N�X�`���̃s�N�Z���̏���
	Out.Out0 = float4(0.1f, 0.1f, 1.0f, 0.7f);
	//�F�ɖ��邳����Z	
	Out.Out0.rgb *= In.Diffuse.rgb * light;
	Out.Out0.a *= In.Diffuse.a;
	//����
	Out.Out0.rgb += Light.Ambient.rgb;
	
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
	specular = pow(specular, 10) * 2;

	Out.Out0.rgb += (specular * ofs);
	
    //�t���l������
	float vdotn = -dot(eyev, normal.rgb);
	float fresnel = 0.02 + (1.0 - 0.02) * pow(1.0 - vdotn, 500);
	fresnel = min(fresnel, 1) * 0.075f;
	Out.Out0.rgb = lerp(Out.Out0.rgb, float3(1.0f, 1.0f, 1.0f), fresnel);
	
	Out.Out1 = In.Position.z;
	
	float2 velocity = (In.Position.xy / In.Position.w) - (In.PrevPosition.xy / In.PrevPosition.w);
	float2 velocityNormalized = clamp(velocity * 0.5 + 0.5, 0.0, 1.0);
	Out.Out2 = velocityNormalized;
}
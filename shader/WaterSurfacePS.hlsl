//WaterSurfacePS.hlsl
//20106_�c���@�@
//24_09_10

#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_Normal : register(t1);
Texture2D g_Env : register(t2);

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
	//���̂�������ւ̃x�N�g��
	float4 lv = normalize(Light.Direction);
		
    //UV�I�t�Z�b�g��K�؂ɐݒ�
	float2 uv1 = In.TexCoord;
	float2 uv2 = In.TexCoord;
	float2 uv3 = In.TexCoord;
	
	uv1 = uv1 * 1.5f - float2(Parameter.y, Parameter.z);
	uv2 = uv2 * 1.0f - float2(Parameter.y, -Parameter.z);
	uv3 = uv3 * 0.5f - float2(Parameter.y, 0.0f);
	
	float2 localnormal1 = g_Normal.Sample(g_SamplerState, uv1).rg;
	float2 localnormal2 = g_Normal.Sample(g_SamplerState, uv2).rg;
	float2 localnormal3 = g_Normal.Sample(g_SamplerState, uv3).rg;
	//RG���x�N�g����
	localnormal1 = (localnormal1 * 2.0f) - 1.0f; //-1�`1�ɂ���
	localnormal2 = (localnormal2 * 2.0f) - 1.0f; //-1�`1�ɂ���
	localnormal3 = (localnormal3 * 2.0f) - 1.0f; //-1�`1�ɂ���
	
	//B�`�����l�����Čv�Z
	float4 normal1 = float4(localnormal1.r, localnormal1.g, sqrt(saturate(1.0f - dot(localnormal1.r, localnormal1.g))), 0.0f);
	float4 normal2 = float4(localnormal2.r, localnormal2.g, sqrt(saturate(1.0f - dot(localnormal2.r, localnormal2.g))), 0.0f);
	float4 normal3 = float4(localnormal3.r, localnormal3.g, sqrt(saturate(1.0f - dot(localnormal3.r, localnormal3.g))), 0.0f);
		
	normal2 += normal1;
	normal2 += normal3;
	normal2 = normalize(normal2);
	
	
	//�^���W�F���g
	float4 tangent = normalize(In.Tangent);
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
	normal = mul(normal2, mat);
	normal = normalize(normal);
	
	//�����v�Z
	float light = 0.5f - 0.5f * dot(lv.xyz, normal.xyz);
	
	//�e�N�X�`���̃s�N�Z���̏���
	Out.Out0 = float4(0.2f, 0.2f, 0.5f, 1.0f);
	//�F�ɖ��邳����Z	
	Out.Out0.rgb *= In.Diffuse.rgb * light;
	Out.Out0.a *= In.Diffuse.a;
	//����
	Out.Out0.rgb += Light.Ambient.rgb;
	
	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev;
	EyeVector(In, eyev);
	
	//�n�[�t�x�N�g�����v�Z
	float3 halfv = lv.xyz + eyev;
	//���K��
	halfv = normalize(halfv);
	
	//�X�y�L����
	float specular = -dot(halfv, normal.xyz);
	specular = saturate(specular);
	specular = pow(specular, 10) * 2;

	Out.Out0.rgb += specular;
	
    //���}�b�v
	float3 refv = reflect(eyev.xyz, normal.xyz);
	refv = normalize(refv);
	
	float2 uv;
	uv.x = -atan2(refv.x, refv.z) / (PI * 2);
	uv.y = acos(refv.y) / PI;
	
	float3 reflect = g_Env.Sample(g_SamplerState, uv);
	
    //�t���l������
	float vdotn = saturate(dot(-eyev, normal.rgb));
	float fresnel = 0.02f + (1.0f - 0.02f) * pow(1.0f - vdotn, 5);
	fresnel = min(fresnel, 1) * 1.0f;
	Out.Out0.rgb = lerp(Out.Out0.rgb, reflect.rgb, fresnel);
	Out.Out0.a *= fresnel * 1.0f;
		
	CreateVelTex(In, Out.Out1);
}
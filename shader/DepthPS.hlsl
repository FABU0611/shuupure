//DepthPS.hlsl
//20106_�c���@�@
//24_09_28

#include "common.hlsl"

Texture2D g_SceneTexture : register(t0);
Texture2D g_BlurTexture : register(t1);
Texture2D<float> g_DepthTexture : register(t2);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out) {
    //�[�x�l
	float4 scene = g_SceneTexture.Sample(g_SamplerState, In.TexCoord);
	float4 blur = g_BlurTexture.Sample(g_SamplerState, In.TexCoord);
	float depthValue = g_DepthTexture.Sample(g_SamplerState, In.TexCoord).r;
	
	float blurFactor;
	
	DepthofField(depthValue, blurFactor);

	//�[�x�Ɋ�Â����O���[�X�P�[���̏o��
	Out.Out0 = lerp(scene, blur, blurFactor);
	Out.Out0 *= In.Diffuse;
	Out.Out0.a = 1.0f;
}
	
 //   //�N���b�v��Ԃ̐[�x���擾
	//float depth = depthValue;

 //   //�[�x�̐��`��
	//float linearDepth = (2.0f * DoF.x) / (DoF.y + DoF.x - depth * (DoF.y - DoF.x));

 //   //0�`1�͈̔͂ɃN�����v
	//linearDepth = clamp(linearDepth, 0.0f, 1.0f);
    
	////outDiffuse = float4(linearDepth, linearDepth, linearDepth, 1.0f);
		
	
	////�œ_�����Ɣ͈͂Ɋ�Â��Ē���
	//float focalDistance = 0.5f; // �œ_����
	//float focalRange = DoF.z; // �s���g�������͈�

	////�O���[�X�P�[���̐[�x���œ_�����Ɋ�Â��ĕϊ�
	//float blurFactor = saturate(abs(linearDepth - focalDistance) / focalRange);

	////0?1�͈̔͂ɃN�����v
	//blurFactor = clamp(blurFactor, 0.0f, 1.0f);
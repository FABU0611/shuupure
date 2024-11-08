//MotionBlurPS.hlsl
//20106_�c���@�@
//24_11_08

#include "common.hlsl"

Texture2D g_SceneTexture : register(t0);
Texture2D g_VelTexture : register(t1);
SamplerState g_SamplerState : register(s0);

float blurStrength = 10.2f; // �u���[���x

void main(in PS_IN In, out PS_OUT Out) {
    // �ʏ�̃V�[���J���[�擾
	float4 sceneColor = g_SceneTexture.Sample(g_SamplerState, In.TexCoord);
    
    // ���x�}�b�v����s�N�Z���̑��x�x�N�g���擾
	float2 velocity = g_VelTexture.Sample(g_SamplerState, In.TexCoord).xy;
    
    // �T���v���ݐ�
	float4 blurredColor = sceneColor;
	int sampleCount = 10;
    
    // ���x�x�N�g���Ɋ�Â��ĕ�����T���v�����O
	for(int i = 1; i < sampleCount; i++) {
		float factor = i / float(sampleCount);
		float2 samplePos = In.TexCoord + normalize(velocity) * factor * blurStrength;
		float4 sampleColor = g_SceneTexture.Sample(g_SamplerState, samplePos);
		blurredColor += sampleColor;
	}
    
    // ���ω�
	blurredColor /= sampleCount;
    
    // �o�̓J���[��ݒ�
	Out.Out0 = blurredColor;
	Out.Out0.a = 1.0f;
}
/*
    // �V�[���J���[���T���v��
float4 sceneColor = g_SceneTexture.Sample(g_SamplerState, TexCoord);
    
    // ���x�x�N�g�����擾
float2 velocity = g_VelocityMap.Sample(g_SamplerState, TexCoord).xy;
    
    // �u���[�����Ƌ������v�Z
float2 blurDirection = normalize(velocity) * blurScale;
    
    // �u���[����
float4 blurredColor = sceneColor;
    for (int i = 1;i < blurSamples; i++){
        float factor = float(i) / float(blurSamples);
        float2 samplePos = TexCoord + blurDirection * factor;
        blurredColor += g_SceneTexture.Sample(g_SamplerState, samplePos);
    }
    
    blurredColor /= float(blurSamples);
    
    return blurredColor;
*/
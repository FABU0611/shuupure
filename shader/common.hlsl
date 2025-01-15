

static float PI = 3.14159265359f;

cbuffer WorldBuffer : register(b0) {
	matrix World;
}
cbuffer ViewBuffer : register(b1) {
	matrix View;
}
cbuffer ProjectionBuffer : register(b2) {
	matrix Projection;
}
cbuffer WorldBuffer : register(b9) {
	matrix PrevWorld;
}
cbuffer ViewBuffer : register(b10) {
	matrix PrevView;
}
cbuffer ProjectionBuffer : register(b11) {
	matrix PrevProjection;
}




struct MATERIAL {
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3) {
	MATERIAL Material;
}




struct LIGHT {
	bool Enable;			//bool�^����float�^�ł��܂�����Ă���Ă���
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	float4 SkyColor;		//�V���F
	float4 GroundColor;		//�n�ʐF
	float4 GroundNormal;	//�n�ʖ@��
	float4 Position;
	float4 PointParam;
	float4 Angle;	
	matrix ViewMatrix;
	matrix ProjectionMatrix[3];
};

cbuffer LightBuffer : register(b4) {
	LIGHT Light;
}


cbuffer CameraBuffer : register(b5) {
	float4 CameraPosition; //�J�����̍��W
}

//�ėp�p�����[�^�󂯎��p
cbuffer ParameterBuffer : register(b6) {
	float4 Parameter; //C���ꂩ�牽���󂯎��
}

//�{�P�p�����[�^�󂯎��p
cbuffer WeightsBuffer : register(b7) {
	float4 Weights[2]; //C���ꂩ�牽���󂯎��
}

//��ʊE�[�x�p�����[�^�󂯎��p
cbuffer DoFBuffer : register(b8) {
	float2 DoF;
}

//�J�X�P�[�h�p�����[�^�󂯎��p
cbuffer SplitBuffer : register(b12) {
	float Split[3];
	float dummy;
}

struct VS_IN {
	float4 Position			: POSITION0;	//�|�W�V�����[��
	float4 Normal			: NORMAL0;		//�m�[�}���[��
	float4 Diffuse			: COLOR0;		//�J���[�[��
	float2 TexCoord			: TEXCOORD0;	//�e�N�X�R�[�h�[��
	float4 Tangent			: TANGENT0;		//�^���W�F���g�[��
};


struct PS_IN {
	float4 Position			: SV_POSITION;	//�s�N�Z���̍��W
	float4 WorldPosition	: POSITION0;	//�s�N�Z���̋�Ԃł̍��W
	float4 Normal			: NORMAL0;		//�s�N�Z���̖@��
	float4 Diffuse			: COLOR0;		//�s�N�Z���̐F
	float2 TexCoord			: TEXCOORD0;	//�s�N�Z���̃e�N�X�`�����W
	float4 Tangent			: TANGENT0;		//�^���W�F���g
	float4 Binormal			: BINORMAL0;	//�o�C�m�[�}��
	float4 CurPosition		: TEXCOORD1;	//���t���[���s�N�Z���̍��W
	float4 PrevPosition		: TEXCOORD2;	//�O�t���[���s�N�Z���̍��W
	float4 LightPosition0	: POSITION1;	//���C�g��ԍ��W
	float4 LightPosition1	: POSITION2;	//���C�g��ԍ��W
	float4 LightPosition2	: POSITION3;	//���C�g��ԍ��W
};
struct PS_OUT {
	float4 Out0				: SV_Target0;	//�f�t�H���g�̂т傤��
	float2 Out1				: SV_Target1;	//���x�}�b�v
};

void HalfVector(in float3 eyev, in float4 lv, out float3 halfv) {
	halfv = eyev + lv.xyz;
	halfv = normalize(halfv);
}

//�J��������̃x�N�g��
void EyeVector(in PS_IN In, out float3 eyevec) {
	eyevec = In.WorldPosition.xyz - CameraPosition.xyz;
	eyevec = normalize(eyevec);
}

//�֊s���v�Z
void Edge(in PS_IN In, in float4 normal, in float range, in float size, inout float4 outDiffuse) {
	float3 eyev;
	EyeVector(In, eyev);
	
	//�֊s����
	float lim = 1.0f - dot(-eyev, normal.xyz);
	//�ǂ̂��炢�Ȃ�G�b�W�ɂ��邩
	if(lim > range) {
		outDiffuse.rgb *= size;
	}
}

//�t�����v�Z
void LimLight(in PS_IN In, in float4 normal, inout float4 outDiffuse) {
	float3 eyev;
	EyeVector(In, eyev);
	
	//���C�g�Ƃ̃x�N�g��lit���v�Z���Ȃ���Ή����݂����ɂȂ�
	float lit = 1.0f - max(0.0f, dot(Light.Direction.xyz, eyev.xyz));
	//�֊s�����𖾂邭����
	float lim = 1.0f - max(0.0f, dot(-eyev, normal.xyz));
	//lit��lim�̖��邳������
	lim *= lit;
	lim = pow(lim, 2);
	
	outDiffuse.rgb += lim;
}

void DepthofField(in float depth, out float value) {
    //�[�x�̐��`��
	float linearDepth = (2.0f * 1.0f) / (1000.0f + 1.0f - depth * (1000.0f - 1.0f));

    //0�`1�͈̔͂ɃN�����v
	linearDepth = clamp(linearDepth, 0.0f, 1.0f);
	
	//�œ_�����Ɣ͈͂Ɋ�Â��Ē���
	float focalDistance = DoF.x; // �œ_����
	float focalRange = DoF.y; // �s���g�������͈�

	//�O���[�X�P�[���̐[�x���œ_�����Ɋ�Â��ĕϊ�
	float blurFactor = saturate(abs(linearDepth - focalDistance) / focalRange);

	//0�`1�͈̔͂ɃN�����v
	blurFactor = clamp(blurFactor, 0.0f, 1.0f);
	
	value = blurFactor;
}

void CreateVelTex(in PS_IN In, out float2 vel) {
	float2 velocity = (In.CurPosition.xy / In.CurPosition.w) - (In.PrevPosition.xy / In.PrevPosition.w);
	velocity.y = -velocity.y;
	float2 velocityNormalized = (velocity + 1.0f) * 0.5f;
	velocityNormalized = clamp(velocityNormalized, 0.39f, 0.69f);
	vel.rg = float2(velocityNormalized);
}

/*
�A���t�@�N���b�v
clip(outDiffuse.a - 0.5f);

�A���t�@�e�B�U
float t = frac((In.TexCoord.x + In.TexCoord.y) * 200.0f);
clip(outDiffuse - f);
*/
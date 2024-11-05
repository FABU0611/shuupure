


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}




struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
    bool Enable; //bool�^����float�^�ł��܂�����Ă���Ă���
    bool3 Dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
    float4 SkyColor; //�V���F
    float4 GroundColor; //�n�ʐF
    float4 GroundNormal; //�n�ʖ@��
    float4 Position;
    float4 PointParam;
    float4 Angle;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}


cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition; //�J�����̍��W
}

//�ėp�p�����[�^�󂯎��p
cbuffer ParameterBuffer : register(b6)
{
    float4 Parameter; //C���ꂩ�牽���󂯎��
}


struct VS_IN
{
    float4 Position : POSITION0; //�|�W�V�����[��
    float4 Normal : NORMAL0; //�m�[�}���[��
    float4 Diffuse : COLOR0; //�J���[�[��
    float2 TexCoord : TEXCOORD0; //�e�N�X�R�[�h�[��
    float4 Tangent : TANGENT0; //�^���W�F���g�[��
};


struct PS_IN
{
    float4 Position : SV_POSITION; //�s�N�Z���̍��W
    float4 WorldPosition : POSITION0; //�s�N�Z���̋�Ԃł̍��W
    float4 Normal : NORMAL0; //�s�N�Z���̖@��
    float4 Diffuse : COLOR0; //�s�N�Z���̐F
    float2 TexCoord : TEXCOORD0; //�s�N�Z���̃e�N�X�`�����W
    float4 Tangent : TANGENT0; //�^���W�F���g
    float4 Binormal : BINORMAL0; //�o�C�m�[�}��
};

void HalfVector(in float3 eyev, in float4 lv, out float3 halfv)
{
    halfv = eyev + lv.xyz;
    halfv = normalize(halfv);
}

//�J��������̃x�N�g��
void EyeVector(in PS_IN In, out float3 eyevec)
{
    eyevec = CameraPosition.xyz - In.WorldPosition.xyz;
    eyevec = normalize(eyevec);
}

//�֊s���v�Z
void Edge(in PS_IN In, in float4 normal, in float range, in float size, inout float4 outDiffuse)
{
    float3 eyev;
    EyeVector(In, eyev);
	
	//�֊s����
    float lim = 1.0f - dot(-eyev, normal.xyz);
	//�ǂ̂��炢�Ȃ�G�b�W�ɂ��邩
    if (lim > range)
    {
        outDiffuse.rgb *= size;
    }
}

//�t�����v�Z
void LimLight(in PS_IN In, in float4 normal, inout float4 outDiffuse)
{
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
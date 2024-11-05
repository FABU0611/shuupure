


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
    bool Enable; //bool型だがfloat型でうまくやってくれている
    bool3 Dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
    float4 SkyColor; //天球色
    float4 GroundColor; //地面色
    float4 GroundNormal; //地面法線
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
    float4 CameraPosition; //カメラの座標
}

//汎用パラメータ受け取り用
cbuffer ParameterBuffer : register(b6)
{
    float4 Parameter; //C言語から何か受け取る
}


struct VS_IN
{
    float4 Position : POSITION0; //ポジションゼロ
    float4 Normal : NORMAL0; //ノーマルゼロ
    float4 Diffuse : COLOR0; //カラーゼロ
    float2 TexCoord : TEXCOORD0; //テクスコードゼロ
    float4 Tangent : TANGENT0; //タンジェントゼロ
};


struct PS_IN
{
    float4 Position : SV_POSITION; //ピクセルの座標
    float4 WorldPosition : POSITION0; //ピクセルの空間での座標
    float4 Normal : NORMAL0; //ピクセルの法線
    float4 Diffuse : COLOR0; //ピクセルの色
    float2 TexCoord : TEXCOORD0; //ピクセルのテクスチャ座標
    float4 Tangent : TANGENT0; //タンジェント
    float4 Binormal : BINORMAL0; //バイノーマル
};

void HalfVector(in float3 eyev, in float4 lv, out float3 halfv)
{
    halfv = eyev + lv.xyz;
    halfv = normalize(halfv);
}

//カメラからのベクトル
void EyeVector(in PS_IN In, out float3 eyevec)
{
    eyevec = CameraPosition.xyz - In.WorldPosition.xyz;
    eyevec = normalize(eyevec);
}

//輪郭を計算
void Edge(in PS_IN In, in float4 normal, in float range, in float size, inout float4 outDiffuse)
{
    float3 eyev;
    EyeVector(In, eyev);
	
	//輪郭部分
    float lim = 1.0f - dot(-eyev, normal.xyz);
	//どのくらいならエッジにするか
    if (lim > range)
    {
        outDiffuse.rgb *= size;
    }
}

//逆光を計算
void LimLight(in PS_IN In, in float4 normal, inout float4 outDiffuse)
{
    float3 eyev;
    EyeVector(In, eyev);
	
	//ライトとのベクトルlitを計算しなければ縁取りみたいになる
    float lit = 1.0f - max(0.0f, dot(Light.Direction.xyz, eyev.xyz));
	//輪郭部分を明るくする
    float lim = 1.0f - max(0.0f, dot(-eyev, normal.xyz));
	//litとlimの明るさを合成
    lim *= lit;
    lim = pow(lim, 2);
	
    outDiffuse.rgb += lim;
}
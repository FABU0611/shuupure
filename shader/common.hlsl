

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
	bool Enable;			//bool型だがfloat型でうまくやってくれている
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	float4 SkyColor;		//天球色
	float4 GroundColor;		//地面色
	float4 GroundNormal;	//地面法線
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
	float4 CameraPosition; //カメラの座標
}

//汎用パラメータ受け取り用
cbuffer ParameterBuffer : register(b6) {
	float4 Parameter; //C言語から何か受け取る
}

//ボケパラメータ受け取り用
cbuffer WeightsBuffer : register(b7) {
	float4 Weights[2]; //C言語から何か受け取る
}

//被写界深度パラメータ受け取り用
cbuffer DoFBuffer : register(b8) {
	float2 DoF;
}

//カスケードパラメータ受け取り用
cbuffer SplitBuffer : register(b12) {
	float Split[3];
	float dummy;
}

struct VS_IN {
	float4 Position			: POSITION0;	//ポジションゼロ
	float4 Normal			: NORMAL0;		//ノーマルゼロ
	float4 Diffuse			: COLOR0;		//カラーゼロ
	float2 TexCoord			: TEXCOORD0;	//テクスコードゼロ
	float4 Tangent			: TANGENT0;		//タンジェントゼロ
};


struct PS_IN {
	float4 Position			: SV_POSITION;	//ピクセルの座標
	float4 WorldPosition	: POSITION0;	//ピクセルの空間での座標
	float4 Normal			: NORMAL0;		//ピクセルの法線
	float4 Diffuse			: COLOR0;		//ピクセルの色
	float2 TexCoord			: TEXCOORD0;	//ピクセルのテクスチャ座標
	float4 Tangent			: TANGENT0;		//タンジェント
	float4 Binormal			: BINORMAL0;	//バイノーマル
	float4 CurPosition		: TEXCOORD1;	//今フレームピクセルの座標
	float4 PrevPosition		: TEXCOORD2;	//前フレームピクセルの座標
	float4 LightPosition0	: POSITION1;	//ライト空間座標
	float4 LightPosition1	: POSITION2;	//ライト空間座標
	float4 LightPosition2	: POSITION3;	//ライト空間座標
};
struct PS_OUT {
	float4 Out0				: SV_Target0;	//デフォルトのびょうが
	float2 Out1				: SV_Target1;	//速度マップ
};

void HalfVector(in float3 eyev, in float4 lv, out float3 halfv) {
	halfv = eyev + lv.xyz;
	halfv = normalize(halfv);
}

//カメラからのベクトル
void EyeVector(in PS_IN In, out float3 eyevec) {
	eyevec = In.WorldPosition.xyz - CameraPosition.xyz;
	eyevec = normalize(eyevec);
}

//輪郭を計算
void Edge(in PS_IN In, in float4 normal, in float range, in float size, inout float4 outDiffuse) {
	float3 eyev;
	EyeVector(In, eyev);
	
	//輪郭部分
	float lim = 1.0f - dot(-eyev, normal.xyz);
	//どのくらいならエッジにするか
	if(lim > range) {
		outDiffuse.rgb *= size;
	}
}

//逆光を計算
void LimLight(in PS_IN In, in float4 normal, inout float4 outDiffuse) {
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

void DepthofField(in float depth, out float value) {
    //深度の線形化
	float linearDepth = (2.0f * 1.0f) / (1000.0f + 1.0f - depth * (1000.0f - 1.0f));

    //0～1の範囲にクランプ
	linearDepth = clamp(linearDepth, 0.0f, 1.0f);
	
	//焦点距離と範囲に基づいて調整
	float focalDistance = DoF.x; // 焦点距離
	float focalRange = DoF.y; // ピントが合う範囲

	//グレースケールの深度を焦点距離に基づいて変換
	float blurFactor = saturate(abs(linearDepth - focalDistance) / focalRange);

	//0～1の範囲にクランプ
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
アルファクリップ
clip(outDiffuse.a - 0.5f);

アルファティザ
float t = frac((In.TexCoord.x + In.TexCoord.y) * 200.0f);
clip(outDiffuse - f);
*/
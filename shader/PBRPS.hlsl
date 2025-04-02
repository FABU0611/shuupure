//PGRPS.hlsl
//20106_�c���@�@
//25_03_27

#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_EnvTexture : register(t1);
SamplerState g_SamplerState : register(s0);


float DistributionGGX(float3 N, float3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	return a2 / (PI * (denom * denom));
}
float3 FresnelSchlick(float cosTheta, float3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
float GeometrySchlickGGX(float NdotV, float roughness) {
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
	return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}
float CG(float3 L, float3 V, float3 H, float3 N){
	float NdotL = max(dot(N, L), 0.0);
	float NdotV = max(dot(N, V), 0.0);
	float NdotH = max(dot(N, H), 0.0);
	float VdotH = max(dot(V, H), 0.0);
	
	float G1 = 2.0 * NdotH * NdotV / VdotH;
	float G2 = 2.0 * NdotH * NdotL / VdotH;
	return min(1.0, min(G1, G2));
}
float geometricOcclusion(float NdotL, float NdotV, float alphaRoughness) {
	float alphaRoughness2 = alphaRoughness * alphaRoughness;
	const float attenuationL = 2.0 * NdotL / (NdotL + sqrt(alphaRoughness2 * alphaRoughness2 + (1.0 - alphaRoughness2 * alphaRoughness2) * (NdotL * NdotL)));
	const float attenuationV = 2.0 * NdotV / (NdotV + sqrt(alphaRoughness2 * alphaRoughness2 + (1.0 - alphaRoughness2 * alphaRoughness2) * (NdotV * NdotV)));
	return attenuationL * attenuationV;
}

float3 Tonemap_ACES(const float3 x) {
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return (x * (a * x + b)) / (x * (c * x + d) + e);
}
float3 PBR_Lighting(float3 albedo, float3 normal, float3 viewDir, float3 lightDir, float3 lightDiffuse, float metallic, float roughness) {
	float3 ambient = Light.Ambient.rgb * albedo;
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotV = max(dot(normal, viewDir), 0.0);
	float3 H = normalize(viewDir + lightDir); // �n�[�t�x�N�g��

    //�������ǂ�����F0��ύX�i�������0.04�A������albedo�j
	float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    //�ePBR�R���|�[�l���g���v�Z
	float3 F = FresnelSchlick(max(dot(normal, viewDir), 0.0), F0);
	float D = DistributionGGX(normal, H, roughness);
	float G = geometricOcclusion(NdotL, NdotV, roughness);

    //�X�y�L��������
	float3 numerator = D * G * F;
	float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
	float3 specular = numerator / denominator;

    //�g�U����
	float3 kS = F;
	float3 kD = 1.0 - kS;
	kD *= 1.0 - metallic; //�����͊g�U���˂Ȃ�
	float3 diffuse = kD * albedo / PI;

    //���C�e�B���O����
	float3 radiance = lightDiffuse * max(dot(normal, lightDir), 0.0) * albedo;
	float3 color = (diffuse + specular) * radiance;
	color = Tonemap_ACES(color);
	return pow(color, (1.0f / 2.2f));
	//return (ambient + specular + radiance);
}
float3 IBL(float3 normal, float3 viewDir, float roughness) {
	float2 uv = 0.0f;
	float3 refv = reflect(-viewDir, normal);
	refv = normalize(refv);
	
	//�p�m���}�}�b�v�p�e�N�X�`�����W
	uv.x = -atan2(refv.x, refv.z) / (PI * 2);
	uv.y = acos(refv.y) / PI;
	
	float4 reflect = g_EnvTexture.Sample(g_SamplerState, uv);
	
	return reflect.rgb * (1.0 - roughness);
}



//float DidtributionGGX(float3 N, float3 H, float roughness) {
//	float a = roughness * roughness;
//	float a2 = a * a;
//	float NdotH = dot(N, H);
//	float NdotH2 = NdotH * NdotH;
//	float d = NdotH2 * (a2 - 1.0f) + 1.0f;
//	return a2 / (PI * d * d);
//}
//float GeometrySchlickGGX(float dot, float roughness) {
//	//float k = (roughness/* + 1.0f*/) * (roughness/* + 1.0f*/) / 2.0f;
//	//return 1.0f / (NdotV * (1.0f - k) + k);
//	float a = roughness * roughness;
//	float k = a * a * 0.5f + 1e-6;
//	return dot / (dot * (1.0f - k) + k);
//}
float GeometrySmithGGX(float roughness, float NdotV, float NdotL) {
	float a = roughness * roughness;
	float k = a * a * 0.5f + 1e-6;
	float ggx1 = NdotL / (NdotL * (1.0f - k) + k);
	float ggx2 = NdotV / (NdotV * (1.0f - k) + k);
	return ggx1 * ggx2;
}


//float CaluculateBeckmann(float m, float t) {
//	float t2 = t * t;
//	float t4 = t * t * t * t;
//	float m2 = m * m;
//	float D = 1.0f / (4.0f * m2 * t4);
//	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
//	return D;
//}
//float CaluculateFresnel(float f0, float u) {
//	return f0 + (1 - f0) * pow(1 - u, 5);
//}
//float CaluculateGeometricDamping(float nh, float nv, float nl, float vh) {
//	return min(1.0f, min(2.0f * nh * nv / vh, 2.0f * nh * nl / vh));
//}
//float CaluculateCookTorranceSpecular(float3 L, float3 V, float3 N, float smooth, float metallic) {
//	float3 H = normalize(L + V);
    
//	float NdotH = saturate(dot(N, H));
//	float VdotH = saturate(dot(V, H));
//	float NdotL = saturate(dot(N, L));
//	float NdotV = saturate(dot(N, V));
    
//	float D = CaluculateBeckmann(metallic, NdotH);
    
//	float F = CaluculateFresnel(smooth, VdotH);
    
//	float G = CaluculateGeometricDamping(NdotH, NdotV, NdotL, VdotH);
    
//	float m = PI * NdotV * NdotH;
    
//	return max(F * D * G / m, 0.0f);
//}
//float CaluculateDiffuseFromFresnel(float3 N, float3 L, float3 V) {
//	float dotNL = saturate(dot(N, L));

//	float dotNV = saturate(dot(N, V));
    
//	return (dotNL * dotNV);
//}

void main(in PS_IN In, out float4 Out : SV_Target0) {
	//�@���Ǝ�������
	float3 normal = normalize(In.Normal.xyz);
	float3 viewDir = normalize(CameraPosition.xyz - In.WorldPosition.xyz);
	float3 lightDirection = normalize(-Light.Direction.xyz);
	
	float NdotL = max(dot(normal, lightDirection), 0.0);
	float NdotV = saturate(dot(normal, viewDir));
	
    //PBR�p�����[�^
	float3 albedo = In.Diffuse.rgb;
	float metallic = Parameter.z;
	float roughness = Parameter.y;
	
	
    //PBR�v�Z
	float3 color = PBR_Lighting(albedo, normal, viewDir, lightDirection, Light.Diffuse.rgb, metallic, roughness);
	//color = max(color, 0.0f);

    //IBL�i���}�b�v�ɂ�锽�ˁj
	float3 env = IBL(normal, viewDir, roughness);
	//color += lerp(env, albedo, length(color));

	Out = float4(color, 1.0);


	////Disney PBR
	//float4 normal = normalize(In.Normal);
	//float4 albedoColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//albedoColor = In.Diffuse;
	//if(Material.TextureEnable) {
	//	albedoColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
	//}
	//float3 specColor = albedoColor;
    
	//float metalic = Parameter.z;
	//float smooth = Parameter.y;
    
	//float3 toEye = CameraPosition.xyz - In.WorldPosition.xyz;
	//toEye = normalize(toEye);

	//float3 lig = 0;
	//for(int ligNo = 0; ligNo < 3; ligNo++) {
	//	float4 lv = Light.Direction;
        
	////�t���l�����˂��l�������g�U����
	//	float diffuseFromFresnel = CaluculateDiffuseFromFresnel(normal.xyz, -lv.xyz, toEye);
        
	////���K�������o�[�g
	//	float NdotL = saturate(dot(normal.xyz, -lv.xyz));
	//	float3 lambertDiffuse = Light.Diffuse.rgb * NdotL / PI;
        
	////�ŏI�I�Ȋg�U����
	//	float3 diffuse = albedoColor.rgb * diffuseFromFresnel * lambertDiffuse;
        
	////kook-torrance���f���ɂ��X�y�L��������
	//	float3 spec = CaluculateCookTorranceSpecular(-lv.xyz, toEye, normal.xyz, smooth, metalic) * Light.Diffuse;
	//	spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metalic);
        
	////�Ȃ߂炩�炪������Ίg�U���˂͎キ�Ȃ�
	//	lig += diffuse * (1.0f - smooth) + spec;
	//}
	
	//lig += Light.Ambient.rgb * albedoColor.rgb;
    
	//Out.rgb = lig;
	//Out.a = albedoColor.a * In.Diffuse.a;
	
	
	//float3 albedo = float3(1.0f, 0.0f, 0.0f);
	//float roughness = Parameter.y;
	//float metallic = Parameter.z;
	
	//float3 normal = normalize(In.Normal);
	//float3 viewDir = normalize(CameraPosition.xyz - In.WorldPosition.xyz);
	//float3 lightDir = normalize(-Light.Direction.xyz);
	//float3 halfDir = normalize(lightDir + viewDir);
	
	////Fresnel-Schlick approximation
	//float3 F0 = float3(0.04f, 0.04f, 0.04f);
	//F0 = lerp(F0, albedo, metallic);
	//float3 fresnel = F0 + (1.0f - F0) * pow(1.0f - dot(viewDir, halfDir), 5.0f);
	
	////NDF(�@�����z�֐�)
	//float NDF = DistributionGGX(normal, halfDir, roughness);
	
	////G(������)
	//float G = GeometrySmith(roughness, dot(normal, viewDir), dot(normal, lightDir));

	////cook-torrance
	//float3 numerator = NDF * G * fresnel;
	//float denominator = 4.0f * max(dot(normal, viewDir), 0.0f) * max(dot(normal, lightDir), 0.0f) + 0.001f;
	//float3 specular = numerator / denominator;
	
	////Lambert
	//float3 kD = (1.0f - fresnel) * (1.0f - metallic);
	
	//float NdotL = max(dot(normal, lightDir), 0.0f);
	//float3 Lo = ((kD * albedo / PI) + specular) * Light.Diffuse.rgb * NdotL;
	
	////Lo = specular;
	//Out.rgb = Lo /* + Light.Ambient.rgb*/;
	//Out.a = In.Diffuse.a;

	
	//float3 N = normalize(In.Normal);
	//float3 V = normalize(CameraPosition.xyz - In.WorldPosition.xyz);
	//float3 L = normalize(-Light.Direction);
	//float3 H = normalize(V + L);
	
	//float metallic = Parameter.z;
	//float roughness = Parameter.y;
	//float3 albedo = In.Diffuse.rgb;
	//float3 directDiffuse = float3(0.0f, 0.0f, 0.0f);
	//float3 directSpecular = float3(0.0f, 0.0f, 0.0f);
	
	//float3 diffuse = lerp(albedo, float3(0.0f, 0.0f, 0.0f), metallic);
	//float3 specular = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);
	
	//float NdotL = max(dot(N, L), 0.0f);
	//float NdotV = max(dot(N, V), 0.0f);
	//float NdotH = saturate(dot(N, H));
	//float VdotH = saturate(dot(V, H));
	//float3 irredianse = Light.Diffuse.rgb * NdotL;
	
	//irredianse *= PI;
	
	
	//directDiffuse += irredianse * (In.Diffuse.rgb / PI);
	
	//float D = DistributionGGX(N, H, roughness);
	//float G = GeometrySmithGGX(roughness, NdotV, NdotL);
	//float3 F = FresnelSchlick(VdotH, specular);
	//float3 specularBRDF = (F * (G * D)) / (4.0f * NdotL * NdotV + 1e-6);
	//directSpecular += irredianse * specularBRDF;
	//float3 outgoingLight = directDiffuse + directSpecular + Light.Ambient.rgb;

	//Out.rgb = outgoingLight;
	//Out.a = In.Diffuse.a;
}
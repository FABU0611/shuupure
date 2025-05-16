//PGRPS.hlsl
//20106_田中　蓮
//25_03_27

//参考：https://www.shadertoy.com/view/XlKSDR


#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_EnvTexture : register(t1);
SamplerState g_SamplerState : register(s0);


float D_GGX(float linearRoughness, float NoH) {
    // Walter et al. 2007, "Microfacet Models for Refraction through Rough Surfaces"
	float oneMinusNoHSquared = 1.0 - NoH * NoH;
	float a = NoH * linearRoughness;
	float k = linearRoughness / (oneMinusNoHSquared + a * a);
	float d = k * k * (1.0 / PI);
	return d;
}

float V_SmithGGXCorrelated(float linearRoughness, float NoV, float NoL) {
    // Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"
	float a2 = linearRoughness * linearRoughness;
	float GGXV = NoL * sqrt((NoV - a2 * NoV) * NoV + a2);
	float GGXL = NoV * sqrt((NoL - a2 * NoL) * NoL + a2);
	return 0.5 / (GGXV + GGXL);
}

float3 F_Schlick(const float3 f0, float VoH) {
    // Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"
	return f0 + (float3(1.0, 1.0, 1.0) - f0) * pow(1.0 - VoH, 5);
}

float F_Schlick(float f0, float f90, float VoH) {
	return f0 + (f90 - f0) * pow(1.0 - VoH, 5);
}

float Fd_Burley(float linearRoughness, float NoV, float NoL, float LoH) {
    // Burley 2012, "Physically-Based Shading at Disney"
	float f90 = 0.5 + 2.0 * linearRoughness * LoH * LoH;
	float lightScatter = F_Schlick(1.0, f90, NoL);
	float viewScatter = F_Schlick(1.0, f90, NoV);
	return lightScatter * viewScatter * (1.0 / PI);
}


float3 Irradiance_SphericalHarmonics(const float3 n) {
    // Irradiance from "Ditch River" IBL (http://www.hdrlabs.com/sibl/archive.html)
	return max(
          float3(0.754554516862612, 0.748542953903366, 0.790921515418539)
        + float3(-0.083856548007422, 0.092533500963210, 0.322764661032516) * (n.y)
        + float3(0.308152705331738, 0.366796330467391, 0.466698181299906) * (n.z)
        + float3(-0.188884931542396, -0.277402551592231, -0.377844212327557) * (n.x)
        , 0.0);
}

float2 PrefilteredDFG_Karis(float roughness, float NoV) {
    // Karis 2014, "Physically Based Material on Mobile"
	const float4 c0 = float4(-1.0, -0.0275, -0.572, 0.022);
	const float4 c1 = float4(1.0, 0.0425, 1.040, -0.040);

	float4 r = roughness * c0 + c1;
	float a004 = min(r.x * r.x, exp2(-9.28 * NoV)) * r.x + r.y;

	return float2(-1.04, 1.04) * a004 + r.zw;
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
float3 OECF_sRGBFast(const float3 x) {
	float a = 1.0 / 2.2;
	return pow(x, float3(a, a, a));
}

void main(in PS_IN In, out float4 Out : SV_Target0) {
	float3 color = float3(0.0f, 0.0f, 0.0f);
	
	float3 N = normalize(In.Normal.xyz);
	float3 V = normalize(CameraPosition.xyz - In.WorldPosition.xyz);
	float3 L = normalize(-Light.Direction.xyz);
	float3 H = normalize(V + L);
	
	float NdotV = abs(dot(N, V)) + 1e-5;
	float NdotL = saturate(dot(N, L));
	float NdotH = saturate(dot(N, H));
	float LdotH = saturate(dot(L, H));
	
	float3 baseColor = In.Diffuse.rgb;
	float roughness = Parameter.y;
	float metallic = Parameter.z;
	
	float intensity = 1.0;			//ライトの強さ
	float indirectIntensity = 0.64;	//間接光の強さ
	
	float linearRoughness = roughness * roughness;
	float3 diffuseColor = (1.0 - metallic) * baseColor.rgb;
	float3 f0 = 0.04 * (1.0 - metallic) + baseColor.rgb * metallic;
	
    // specular BRDF
	float D = D_GGX(linearRoughness, NdotH);
	float VS = V_SmithGGXCorrelated(linearRoughness, NdotV, NdotL);
	float3 F = F_Schlick(f0, LdotH);
	float3 Fr = (D * VS) * F;
	
    // diffuse BRDF
	float3 Fd = diffuseColor * Fd_Burley(linearRoughness, NdotV, NdotL, LdotH);
		
	color = Fd + Fr;
	color *= (intensity * NdotL) * Light.Diffuse.rgb;
	
    // diffuse indirect
	float3 indirectDiffuse = Irradiance_SphericalHarmonics(N) * (1.0 / PI);
	indirectDiffuse += Light.Ambient.rgb;
	float3 indirectSpecular = float3(0.5, 0.5, 0.5);
	
    // indirect contribution
	float2 dfg = PrefilteredDFG_Karis(roughness, NdotV);
	float3 specularColor = f0 * dfg.x + dfg.y;
	float3 ibl = diffuseColor * indirectDiffuse + indirectSpecular * specularColor;

	color += ibl * indirectIntensity;

	//color = Tonemap_ACES(color);
	color = OECF_sRGBFast(color);
	
	Out = float4(color, In.Diffuse.a);
}
//ShaderManager.h
//20106_�c���@�@
//24_12_11
#pragma once

#include <string>
#include <unordered_map>
#include <d3d11.h>

enum class ShaderName : short {
	Unlit,
	Water,
	Normallit,
	Tangentlit,
	Dirlit,
	Particle,
	ParticleInstance,
	Instance,
	DepthShadow,
	PBR,
	CheckDoF,
	Gaussian,
	MotionBlur,
};

struct ShaderPack {
	ID3D11PixelShader* pixelShader = NULL;
	ID3D11VertexShader* vertexShader = NULL;
	ID3D11InputLayout* vertexLayout = NULL;
};

class Shader {
	static std::unordered_map<ShaderName, ShaderPack*>	_shaders;

public:
	Shader() {}
	~Shader() {}

	static void LoadShader();
	static void UninitAll();

	static void SetShader(const ShaderName& name);
};
//ShaderManager.h
//20106_ìcíÜÅ@ò@
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
};

struct ShaderPack {
	ID3D11PixelShader* pixelShader = NULL;
	ID3D11VertexShader* vertexShader = NULL;
	ID3D11InputLayout* vertexLayout = NULL;
};

class Shader {
	static std::unordered_map<ShaderName, ShaderPack*>	_shaders;

	static void AddShader(const std::string& VSpath, const std::string& PSpath, const ShaderName& name);

	static ShaderPack* GetShader(const ShaderName& name);
public:
	Shader() {}
	~Shader() {}

	static void LoadShader();
	static void UninitAll();

	static void SetShader(const ShaderName& name);
};
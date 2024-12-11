//Shaders.cpp
//20106_�c���@�@
//24_12_11
#include "ShaderManager.h"
#include "Main.h"
#include "Renderer.h"

std::unordered_map<ShaderName, ShaderPack*>Shader::_shaders;

void Shader::AddShader(const std::string& VSpath, const std::string& PSpath, const ShaderName& name) {
	ID3D11PixelShader* pixelsharder{};
	ID3D11VertexShader* vertexsharder{};
	ID3D11InputLayout* vertexlayout{};

	Renderer::CreateVertexShader(&vertexsharder, &vertexlayout,
		VSpath.c_str());

	Renderer::CreatePixelShader(&pixelsharder,
		PSpath.c_str());

	_shaders.emplace(name, new ShaderPack{ pixelsharder, vertexsharder, vertexlayout });
}

void Shader::LoadShader() {
	AddShader("shader\\UnlitTextureVS.cso", "shader\\UnlitTexturePS.cso", ShaderName::unlit);
	AddShader("shader\\TangentNormalLightingVS.cso", "shader\\WaterSurfacePS.cso", ShaderName::water);
	AddShader("shader\\NormalLightingVS.cso", "shader\\NormalLightingPS.cso", ShaderName::lit);
	AddShader("shader\\TangentNormalLightingVS.cso", "shader\\TangentNormalLightingPS.cso", ShaderName::tangentlit);
	AddShader("shader\\VertexLightingVS.cso", "shader\\VertexLightingPS.cso", ShaderName::dirlit);
}

void Shader::UninitAll() {
    for (auto& [name, pack] : _shaders) {
        if (pack->pixelShader) {
            pack->pixelShader->Release();
            pack->pixelShader = nullptr;
        }
        if (pack->vertexShader) {
            pack->vertexShader->Release();
            pack->vertexShader = nullptr;
        }
        if (pack->vertexLayout) {
            pack->vertexLayout->Release();
            pack->vertexLayout = nullptr;
        }
        delete pack; //new�Ŋm�ۂ���ShaderPack�̉��
    }
    _shaders.clear(); //�}�b�v���N���A
}

ShaderPack* Shader::GetShader(const ShaderName& name) {
    if (_shaders.contains(name)) {
        return _shaders.at(name);
    }
    else {
        return _shaders.at(ShaderName::unlit);
    }
}

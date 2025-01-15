//Shaders.cpp
//20106_田中　蓮
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
	AddShader("shader\\UnlitTextureVS.cso", "shader\\UnlitTexturePS.cso", ShaderName::Unlit);
	AddShader("shader\\TangentNormalLightingVS.cso", "shader\\WaterSurfacePS.cso", ShaderName::Water);
	AddShader("shader\\NormalLightingVS.cso", "shader\\NormalLightingPS.cso", ShaderName::Normallit);
	AddShader("shader\\TangentNormalLightingVS.cso", "shader\\TangentNormalLightingPS.cso", ShaderName::Tangentlit);
	AddShader("shader\\VertexLightingVS.cso", "shader\\VertexLightingPS.cso", ShaderName::Dirlit);
    AddShader("shader\\UnlitTextureVS.cso", "shader\\ParticlePS.cso", ShaderName::Particle);
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
        delete pack; //newで確保したShaderPackの解放
    }
    _shaders.clear(); //マップをクリア
}

ShaderPack* Shader::GetShader(const ShaderName& name) {
    if (_shaders.contains(name)) {
        return _shaders.at(name);
    }
    else {
        return _shaders.at(ShaderName::Unlit);
    }
}

void Shader::SetShader(const ShaderName& name) {
    ShaderPack* shader = GetShader(name);

    //入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(shader->vertexLayout);

    //シェーダ設定
    Renderer::GetDeviceContext()->VSSetShader(shader->vertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(shader->pixelShader, NULL, 0);
}

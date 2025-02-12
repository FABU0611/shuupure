//ParticleEmitter.cpp
//20106_田中　蓮
//2024_07_05
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "ParticleEmitter.h"
#include "C_Sprite3D.h"
#include "Scene.h"
#include <random>
#include <vector>
#include "ShaderManager.h"

//初期化処理
void ParticleEmitter::Init() {
	Sprite3D* sprite = AddComponent<Sprite3D>(this);
	sprite->SetDrawMode(DrawMode3D::Particle);
	sprite->LoadTexture(_texname);

	//ストラクチャードバッファ
	{
		_particleinstancedata = new PARTICLE_INSTANCE[PARTICLE_MAX];

		for (int i = 0; i < PARTICLE_MAX; i++) {
			_particleinstancedata[i].Pos = _particle[i].Pos;
			_particleinstancedata[i].Size = _particle[i].Size;
			_particleinstancedata[i].Color = _particle[i].Color;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(PARTICLE_INSTANCE) * PARTICLE_MAX;
		bd.StructureByteStride = sizeof(PARTICLE_INSTANCE);
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = _particleinstancedata;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_particlebuffer);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = PARTICLE_MAX;
		Renderer::GetDevice()->CreateShaderResourceView(_particlebuffer, &srvd, &_particleSRV);
	}
}

//終了処理
void ParticleEmitter::Uninit() {
	delete[] _particleinstancedata;
	_particleSRV->Release();
	_particlebuffer->Release();
	for (auto c : _components) {
		c->Uninit();
	}
}

void ParticleEmitter::UpdateParticle() {
	if (!_particleinstancedata) {
		return;
	}
	std::vector<PARTICLE_INSTANCE> bufferdata;

	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (!_particle[i].Enable) {
			continue;
		}
		PARTICLE_INSTANCE buffer;
		buffer.Pos = _particle[i].Pos - GetPosition();
		buffer.Size = _particle[i].Size;
		buffer.Color = _particle[i].Color;
		bufferdata.push_back(buffer);
	}
	if (bufferdata.empty()) {
		return;
	}
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Renderer::GetDeviceContext()->Map(_particlebuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, bufferdata.data(), sizeof(PARTICLE_INSTANCE)* bufferdata.size());
	Renderer::GetDeviceContext()->Unmap(_particlebuffer, 0);
	_instancecount = bufferdata.size();

	//ストラクチャードバッファ設定
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &_particleSRV);
}

//描画処理
void ParticleEmitter::Draw() {
	Shader::SetShader(ShaderName::ParticleInstance);
	UpdateParticle();
	for (auto c : _components) {
		c->Draw();
	}
}

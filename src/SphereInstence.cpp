//SphereInstance.h
//20106_田中　蓮
//25_01_23
#include "SphereInstence.h"
#include "Renderer.h"
#include "C_ModelRenderer.h"
#include "Manager.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "Camera.h"

const float SphereInstance::INSTANCE_DISTANCE = 25.0f;

void SphereInstance::Init() {
	GetScale() *= 1.5f;
	SetPosition({ 40.0f, 0.0f, 40.0f });

	ModelRenderer* model = AddComponent<ModelRenderer>(this);
	model->Load("asset\\model\\sphere.obj");

	//ストラクチャードバッファ
	{
		_instancepos = new XMFLOAT3[INSTANCE_NUM];

		int i = 0;
		for (int x = 0; x < INSTANCE_NUM / INSTANCE_X_NUM; x++) {
			for (int z = 0; z < INSTANCE_NUM / INSTANCE_Z_NUM; z++) {
				_instancepos[i] = XMFLOAT3((x - (INSTANCE_X_NUM * 0.5f)) * INSTANCE_DISTANCE, 0.0f, (z - (INSTANCE_Z_NUM * 0.5f)) * INSTANCE_DISTANCE);
				i++;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(XMFLOAT3) * INSTANCE_NUM;
		bd.StructureByteStride = sizeof(XMFLOAT3);
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = _instancepos;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_posbuffer);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = INSTANCE_NUM;
		Renderer::GetDevice()->CreateShaderResourceView(_posbuffer, &srvd, &_posSRV);
	}

	model->SetInstanceNum(INSTANCE_NUM);
}

void SphereInstance::Uninit() {
	delete[] _instancepos;
	_posSRV->Release();
	_posbuffer->Release();
	for (auto c : _components) {
		c->Uninit();
	}
}

void SphereInstance::Update() {}

void SphereInstance::Draw() {
	//インスタンスごとの視錐台カリング
	Scene* scene = Manager::GetScene();
	if (!scene) {
		return;
	}
	Camera* camera = scene->GetGameobject<Camera>();
	if (!camera) {
		return;
	}
	ModelRenderer* model = GetComponent<ModelRenderer>();
	const float& rad = model->GetRadius() * 1.5f;

	std::vector<XMFLOAT3> visivlepos;
	for (int idx : camera->CheckViewInstance(_instancepos, GetPosition(), INSTANCE_NUM, GetScale()* rad)) {
		visivlepos.push_back(_instancepos[idx]);
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(_posbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, visivlepos.data(), sizeof(XMFLOAT3) * visivlepos.size());
	Renderer::GetDeviceContext()->Unmap(_posbuffer, 0);

	model->SetInstanceNum((int)visivlepos.size());


	Shader::SetShader(ShaderName::Instance);

	//ストラクチャードバッファ設定
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &_posSRV);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world, _prevworld);
	
	for (auto c : _components) {
		c->Draw();
	}
}

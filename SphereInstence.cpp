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
	GetPosition().x = 50.0f;

	ModelRenderer* model = AddComponent<ModelRenderer>(this);
	model->Load("asset\\model\\sphere.obj");

	//ストラクチャードバッファ
	{
		XMFLOAT3* pos = new XMFLOAT3[INSTANCE_NUM];

		int i = 0;
		for (int x = 0; x < INSTANCE_NUM / INSTANCE_X_NUM; x++) {
			for (int z = 0; z < INSTANCE_NUM / INSTANCE_Z_NUM; z++) {
				pos[i] = GetPosition() + XMFLOAT3((x - (INSTANCE_X_NUM * 0.5f)) * INSTANCE_DISTANCE, 0.0f, (z - (INSTANCE_Z_NUM * 0.5f)) * INSTANCE_DISTANCE);
				i++;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) * INSTANCE_NUM;
		bd.StructureByteStride = sizeof(XMFLOAT3);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pos;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_posbuffer);

		delete[] pos;

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
	_posSRV->Release();
	_posbuffer->Release();
	for (auto c : _components) {
		c->Uninit();
	}
}

void SphereInstance::Update() {}

void SphereInstance::Draw() {
	Scene* scene = Manager::GetScene();
	if (!scene) {
		return;
	}
	Camera* camera = scene->GetGameobject<Camera>();
	if (!camera) {
		return;
	}
	const float& rad = GetComponent<ModelRenderer>()->GetRadius();
	if (!camera->ChackView(GetPosition(), rad * GetScale().x)) {
		return;
	}

	Shader::SetShader(ShaderName::Instance);

	//ストラクチャードバッファ設定
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &_posSRV);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;

	if (Manager::GetisDrawFromLight()) {
		Renderer::SetWorldMatrix(world);
	}
	else {
		Renderer::SetWorldMatrix(world, _prevworld);
	}

	for (auto c : _components) {
		c->Draw();
	}
}

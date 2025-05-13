//PBRObject.cpp
//20106_田中　蓮
//25_01_16
#include "PBRObject.h"
#include "Renderer.h"
#include "Manager.h"
#include "Camera.h"
#include "C_ModelRenderer.h"
#include "G_Slidebar.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "GUIManager.h"
#include "Input.h"


void PBRObject::Init() {
	ModelRenderer* model = AddComponent<ModelRenderer>(this);
	model->Load("asset\\model\\sphere.obj");

	SetPosition({ 0.0f, 10.0f, 0.0f });
	SetScale({ 1.0f, 1.0f, 1.0f });
	_param = { 0.0f, 0.5f, 0.5f, 0.0f };

	_envtex = TextureManager::GetInstance()->LoadTexture(L"asset\\model\\sky02.jpg");
}

void PBRObject::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}
}

void PBRObject::Update() {
	if (Input::GetKeyPress('Z')) {
		_param.z += 0.01f;
		_param.z = std::min(_param.z, 1.0f);
	}
	else if (Input::GetKeyPress('X')) {
		_param.z -= 0.01f;
		_param.z = std::max(_param.z, 0.0f);
	}
	if (Input::GetKeyPress('C')) {
		_param.y += 0.01f;
		_param.y = std::min(_param.y, 1.0f);
	}
	else if (Input::GetKeyPress('V')) {
		_param.y -= 0.01f;
		_param.y = std::max(_param.y, 0.0f);
	}

	for (auto c : _components) {
		c->Update();
	}
}

void PBRObject::Draw() {
	Scene* scene = Manager::GetScene();
	if (!scene) {
		return;
	}
	Camera* camera = scene->GetGameobject<Camera>();
	if (!camera) {
		return;
	}
	const float& rad = GetComponent<ModelRenderer>()->GetRadius();
	if (!camera->ChackView(GetPosition(), GetScale() * rad)) {
		return;
	}

	Renderer::SetParameter(_param);

	Shader::SetShader(ShaderName::PBR);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world, _prevworld);

	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &_envtex);

	for (auto c : _components) {
		c->Draw();
	}
}

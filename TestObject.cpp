//TestObject.cpp
//20106_田中　蓮
//25_01_16
#include "TestObject.h"
#include "Renderer.h"
#include "Manager.h"
#include "C_ModelRenderer.h"
#include "Time.h"
#include "ShaderManager.h"

TestObject::TestObject(const XMFLOAT3& pos) {
	SetPosition(pos);
	SetScale({ 1.0f, 10.0f, 1.0f });
}

void TestObject::Init() {
	ModelRenderer* model = AddComponent<ModelRenderer>(this);
	model->Load("asset\\model\\box.obj");

	_time = 0.0f;

	_defpos = GetPosition();
}

void TestObject::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}
}

void TestObject::Update() {
	_time += Time::GetDeltaTime();

	if (_time >= XM_PI * 2) {
		_time = 0.0f;
	}

	GetPosition().z = 10 * sin(_time * 5) + _defpos.y;
	//GetRotation().x += 0.1f;

	for (auto c : _components) {
		c->Update();
	}
}

void TestObject::Draw() {
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

	Shader::SetShader(ShaderName::Unlit);

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

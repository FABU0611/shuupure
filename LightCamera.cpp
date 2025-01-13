//LightCamera.cpp
//20106_ìcíÜÅ@ò@
//25_01_13
#include "LightCamera.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include <vector>

void LightCamera::Init() {
	SetPosition({ 0.0f, 1.5f, 3.0f });
	SetRotation({ 0.0f, 0.5f, 0.0f });
	_target = { 0.0f, 0.5f, 0.0f };
	_up = { 0.0f, 1.0f, 0.0f };

	_light.Direction = { -1.0f, -1.0f, -0.5f, 0.0f };
	_light.Direction = VectorNormalize(_light.Direction);
	_light.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	_light.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void LightCamera::Uninit() {}

void LightCamera::Update() {
}

void LightCamera::Draw() {
	Scene* scene = Manager::GetScene();
	Camera* camera{};
	if (scene) {
		camera = scene->GetGameobject<Camera>();
	}
	if (!camera) {
		return;
	}
	_light.Enable = true;

	_target = camera->GetPosition();

	XMVECTOR dir = XMVector4Normalize(XMLoadFloat4(&_light.Direction));
	XMVECTOR tgt = XMLoadFloat3(&_target);
	XMVECTOR lightpos = tgt - dir * 10.0f;

	XMMATRIX lightview = XMMatrixLookAtLH(lightpos, tgt, XMLoadFloat3(&_up));


	float nearZ = 1.0f;
	float farZ = camera->GetCascade()[0];
	std::vector<XMVECTOR> corners = camera->GetCornersWorldSpace(nearZ, farZ);

	//ç≈ëÂÇ∆ç≈è¨Çèâä˙âª
	XMVECTOR min = XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	XMVECTOR max = XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (const auto& corner : corners) {
		XMVECTOR tranceformedcorner = XMVector3Transform(corner, lightview);
		min = XMVectorMin(min, tranceformedcorner);
		max = XMVectorMax(max, tranceformedcorner);
	}


	//XMMATRIX lightprojection = XMMatrixOrthographicLH(250.0f, 250.0f, 1.0f, 500.0f);		//ê≥éÀâe
	XMMATRIX lightprojection = XMMatrixOrthographicOffCenterLH(
								XMVectorGetX(min), XMVectorGetX(max),
								XMVectorGetY(min), XMVectorGetY(max),
								XMVectorGetZ(min), XMVectorGetZ(max));		//ê≥éÀâe

	_light.ViewMatrix = XMMatrixTranspose(lightview);
	_light.ProjectionMatrix = XMMatrixTranspose(lightprojection);
	Renderer::SetLight(_light);

	Renderer::SetViewMatrix(lightview);
	Renderer::SetProjectionMatrix(lightprojection);
}

//LightCamera.cpp
//20106_田中　蓮
//25_01_13
#include "LightCamera.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "CheckTexture.h"
#include "ShaderManager.h"
#include <vector>

void LightCamera::Init() {
	SetPosition({ 0.0f, 1.5f, 3.0f });
	SetRotation({ 0.0f, 0.5f, 0.0f });
	_target = { 0.0f, 0.5f, 0.0f };

	_light.Direction = { -0.5f, -1.0f, -0.5f, 0.0f };
	_light.Direction = VectorNormalize(_light.Direction);
	_light.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	_light.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	std::vector<ID3D11ShaderResourceView*> srv(Renderer::CASCADE_NUM);
	for (int i = 0; i < Renderer::CASCADE_NUM; i++) {
		srv[i] = (Renderer::GetCameraDepthTexture(i));
	}
	Manager::GetScene()->AddGameobject<CheckTexture>(Layer::UI, "LightDepth", srv, ShaderName::Unlit, XMFLOAT3(SCREEN_HEIGHT, SCREEN_HEIGHT, 0.0f));
}

void LightCamera::Uninit() {}

void LightCamera::Update() {}

void LightCamera::Draw() {
	//シーンのカメラを取得
	Scene* scene = Manager::GetScene();
	Camera* camera{};
	if (scene) {
		camera = scene->GetGameobject<Camera>();
	}
	if (!camera) {
		return;
	}

	//カスケードの切れ目を取得
	int index = Manager::GetCascadeIndex();
	float nearZ = 1.0f;
	if (index == 0) {
		nearZ = 35.0f;
	}
	else {
		nearZ = camera->GetCascade()[index - 1];
	}
	float farZ = camera->GetCascade()[index];


	_target = camera->GetPosition();

	XMVECTOR dir = XMVector4Normalize(XMLoadFloat4(&_light.Direction));
	XMVECTOR tgt = XMLoadFloat3(&_target);
	XMVECTOR lightpos = tgt - dir;

	XMMATRIX lightview = XMMatrixLookAtLH(lightpos, tgt, XMVectorSet(0, 1, 0, 0));


	//視錐台の8頂点の最大と最小を計算
	std::vector<XMVECTOR> corners = camera->GetCornersWorldSpace(nearZ * -1.2f, farZ);

	XMVECTOR min = XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	XMVECTOR max = XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (const auto& corner : corners) {
		XMVECTOR tranceformedcorner = XMVector3Transform(corner, lightview);
		min = XMVectorMin(min, tranceformedcorner);
		max = XMVectorMax(max, tranceformedcorner);
	}


	XMMATRIX lightprojection = XMMatrixOrthographicOffCenterLH(
								XMVectorGetX(min), XMVectorGetX(max),
								XMVectorGetY(min), XMVectorGetY(max),
								XMVectorGetZ(min), XMVectorGetZ(max));		//正射影

	_light.Enable = true;
	_light.ViewMatrix[index] = XMMatrixTranspose(lightview);
	_light.ProjectionMatrix[index] = XMMatrixTranspose(lightprojection);
	Renderer::SetLight(_light);

	Renderer::SetViewMatrix(lightview);
	Renderer::SetProjectionMatrix(lightprojection);
}

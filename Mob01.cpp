//Mob01.cpp
//20106_�c���@�@
//24_09_17
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Input.h"
#include "Mob01.h"
#include "C_AnimationModel.h"
#include "C_Transform.h"

Mob01::Mob01(const XMFLOAT3& pos, const XMFLOAT3& scl){
	SetPosition(pos);
	SetScale(scl);
}

void Mob01::Init(){
	AddComponent<AnimationModel>(this);

	//���f���ǂݍ���
	GetComponent<AnimationModel>()->Load("asset\\model\\Adam.fbx");
	GetComponent<AnimationModel>()->LoadAnimation("asset\\model\\Sitting.fbx", "Sit");

	//�V�F�[�_�[�Z�b�g
	Renderer::CreateVertexShader(&_vertexShader, &_vertexLayout,
		"shader\\NormalLightingVS.cso");

	Renderer::CreatePixelShader(&_pixelShader,
		"shader\\NormalLightingPS.cso");

	SetRotation({ 0.0f, 0.0f, 0.0f });
}

void Mob01::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexShader->Release();
	_pixelShader->Release();
	_vertexLayout->Release();
}

void Mob01::Update() {
	for (auto c : _components) {
		c->Update();
	}
}

void Mob01::Draw(){
	GetComponent<AnimationModel>()->Update("Sit", _animationFrame);
	_animationFrame++;

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_vertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	rot = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	for (auto c : _components) {
		c->Draw();
	}
}
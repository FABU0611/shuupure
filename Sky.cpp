//sky.cpp
//20106_�c���@�@
//24_06_05
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Sky.h"
#include "C_Transform.h"
#include "C_AnimationModel.h"

void Sky::Init() {
	AddComponent<AnimationModel>(this);

	//���f���ǂݍ���
	GetComponent<AnimationModel>()->Load("asset\\model\\skydoom.fbx");


	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\unlitTexturePS.cso");
}

void Sky::Uninit() {
	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

void Sky::Update() {
}

void Sky::Draw() {
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

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

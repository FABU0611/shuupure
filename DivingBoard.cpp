//DivingBoard.h	��э��ݑ�
//20106_�c���@�@
//24_09_07

#include "DivingBoard.h"
#include "Renderer.h"
#include "C_AnimationModel.h"

void DivingBoard::Init(){
	AddComponent<AnimationModel>(this);

	//���f���ǂݍ���
	GetComponent<AnimationModel>()->Load("asset\\model\\jumpdai.fbx");

	//�V�F�[�_�[�Z�b�g
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\DepthShadowVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\DepthShadowPS.cso");
}

void DivingBoard::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

void DivingBoard::Update(){
	for (auto c : _components) {
		c->Update();
	}
}

void DivingBoard::Draw(){
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
	Renderer::SetWorldMatrix(world, _prevworld);

	for (auto c : _components) {
		c->Draw();
	}
}

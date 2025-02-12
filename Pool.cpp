//DivingBoard.h	��э��ݑ�
//20106_�c���@�@
//24_09_07

#include "Pool.h"
#include "Renderer.h"
#include "C_AnimationModel.h"
#include "ShaderManager.h"

void Pool::Init(){
	AddComponent<AnimationModel>(this);

	//���f���ǂݍ���
	GetComponent<AnimationModel>()->Load("asset\\model\\pool.fbx");
}

void Pool::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}
}

void Pool::Update(){
	for (auto c : _components) {
		c->Update();
	}
}

void Pool::Draw(){
	Shader::SetShader(ShaderName::DepthShadow);

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

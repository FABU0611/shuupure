//CheckTexture.cpp
//20106_�c���@�@
//25_02_07
#include "CheckTexture.h"
#include "C_Sprite2D.h"
#include "G_CheckBox.h"
#include "GUIManager.h"

CheckTexture::CheckTexture(const std::string& label, const unsigned short& dispnum, std::vector<ID3D11ShaderResourceView*> srv, const ShaderName& shadername)
:_dispnum(dispnum), _srv(srv), _shadername(shadername) {
	int size = GUIManager::GetInstance()->GetGUISize();
	_index = GUIManager::GetInstance()->AddGUI<CheckBox>(label);
}

CheckTexture::CheckTexture(const std::string& label, ID3D11ShaderResourceView* srv, const ShaderName& shadername) :
	_dispnum(1), _shadername(shadername) {
	_srv.push_back(srv);
	int size = GUIManager::GetInstance()->GetGUISize();
	_index = GUIManager::GetInstance()->AddGUI<CheckBox>(label);
}

//����������
void CheckTexture::Init() {
	Sprite2D* sprite = AddComponent<Sprite2D>(this);
	sprite->SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	sprite->SetDrawMode(DrawMode::Multiply);
	sprite->Sprite::CreateVertexBuffer(_dispnum);
}

//�I������
void CheckTexture::Uninit() {
	for (auto& comp : _components) {
		comp->Uninit();
	}
}

//�X�V����
void CheckTexture::Update() {
	for (auto& comp : _components) {
		comp->Update();
	}
}

//�`�揈��
void CheckTexture::Draw() {
	CheckBox* checkbox = dynamic_cast<CheckBox*>(GUIManager::GetInstance()->GetGUI(_index));
	if (!checkbox) {
		return;
	}
	if (!checkbox->OnClicked(VK_LBUTTON)) {
		return;
	}
	Shader::SetShader(_shadername);

	Sprite2D* sprite = GetComponent<Sprite2D>();
	if (!sprite) {
		return;
	}
	sprite->SetDispSRV(_srv);
	//sprite->Sprite::_texture = _srv[0];
	sprite->Draw();
}
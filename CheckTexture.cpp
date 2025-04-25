//CheckTexture.cpp
//20106_田中　蓮
//25_02_07
#include "CheckTexture.h"
#include "C_Sprite2D.h"
#include "G_CheckBox.h"
#include "GUIManager.h"

CheckTexture::CheckTexture(const std::string& label, std::list<ID3D11ShaderResourceView*>& srv, const ShaderName& shadername, const XMFLOAT3& dispsize)
:_dispnum(static_cast<unsigned short>(srv.size())), _srv(std::move(srv)), _shadername(shadername) {
	_checkbox = GUIManager::GetInstance()->AddGUI<CheckBox>(label);

	Sprite2D* sprite = AddComponent<Sprite2D>(this);
	sprite->SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	sprite->SetDrawMode(DrawMode::Multiply);
	sprite->Sprite::CreateVertexBuffer(_dispnum);
	sprite->SetSize(dispsize);
}

CheckTexture::CheckTexture(const std::string& label, ID3D11ShaderResourceView* srv, const ShaderName& shadername, const XMFLOAT3& dispsize) :
	_dispnum(1), _shadername(shadername) {
	_srv.push_back(srv);
	_checkbox = GUIManager::GetInstance()->AddGUI<CheckBox>(label);

	Sprite2D* sprite = AddComponent<Sprite2D>(this);
	sprite->SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	sprite->SetDrawMode(DrawMode::Multiply);
	sprite->Sprite::CreateVertexBuffer(_dispnum);
	sprite->SetSize(dispsize);
}

//終了処理
void CheckTexture::Uninit() {
	for (auto& comp : _components) {
		comp->Uninit();
	}
	_srv.clear();
}

//更新処理
void CheckTexture::Update() {
	for (auto& comp : _components) {
		comp->Update();
	}
}

//描画処理
void CheckTexture::Draw() {
	if (!_checkbox) {
		return;
	}
	if (!_checkbox->OnClicked(MOUSE_BUTTON::MOUSE_LBUTTON)) {
		return;
	}
	Shader::SetShader(_shadername);

	Sprite2D* sprite = GetComponent<Sprite2D>();
	if (!sprite) {
		return;
	}
	sprite->SetDispSRV(_srv);
	sprite->Draw();
}
//G_Slidebar.cpp
//20106_田中　蓮
//25_02_26
#include "Text.h"
#include "G_Slidebar.h"
#include "C_Sprite2D.h"
#include "Input.h"
#include "Manager.h"
#include "GUIManager.h"
#include "TextManager.h"


const XMFLOAT3 Slidebar::_slidersize = { 200.0f, 25.0f, 0.0f };	//スライダーバーのサイズ

//コンストラクタ
Slidebar::Slidebar(const std::string& label, const XMFLOAT3& pos, const float& max, const float& min, float* value)
:GUI(pos, _slidersize, L"asset\\texture\\slidebar_bar.png"), _max(max), _min(min), _value(value){
	//値からハンドル位置を計算
	float normalized = (*_value - _min) / (_max - _min);
	_handleoffset.x = -_slidersize.x * 0.5f + _slidersize.x * normalized;

	//値を最大値と最小値の間に収める
	*_value = std::max(_min, std::min(_max, *_value));

	//値を右側に表示
	_valuetext = TextManager::GetInstance()->AddText<Text>(_size.y, D2D1::ColorF::Black, TextAnchor::CenterLeft);
	_valuetext->SetString(std::to_string(*_value));
	_valuetext->SetPosition({ pos.x + _size.x * 0.5f + _slidersize.y, pos.y, 0.0f });

	//ラベルを左側に表示
	_labeltext = TextManager::GetInstance()->AddText<Text>(_size.y, D2D1::ColorF::Black, TextAnchor::CenterRight);
	_labeltext->SetString(label);
	_labeltext->SetPosition({ pos.x - _size.x * 0.5f - _slidersize.y, pos.y, 0.0f });
}

//初期化処理
void Slidebar::Init() {
	GUI::Init();

	//スライダーハンドルのテクスチャを読み込み
	_handle = AddComponent<Sprite2D>(this);
	_handle->LoadTexture(L"asset\\texture\\slidebar_handle.png");
	_handle->SetDrawMode(DrawMode::Color);
	_handle->SetSize({ _size.y, _size.y, 0.0f });
}

//更新処理
void Slidebar::Update() {
	POINT mousepos = Input::GetMousePosition();
	bool ishover = IsHovered(mousepos);


	//マウスホイールで値を変更
	if (ishover) {
		*_value += Input::GetMouseWheelDelta() * 0.01f;
	}

	//ドラッグで値を変更
	XMFLOAT3 pos = GetPosition();
	if (_isdrag) {
		float left = pos.x - _size.x * 0.5f;
		float right = pos.x + _size.x * 0.5f;
		float x = std::max(left, std::min((float)mousepos.x, right));

		*_value = _min + (_max - _min) * ((x - left) / (_size.x));
	}
	*_value = std::max(_min, std::min(_max, *_value));

	//値からハンドル位置を計算
	float normalized = (*_value - _min) / (_max - _min);
	_handleoffset.x = -_size.x * 0.5f + _size.x * normalized;
	_valuetext->SetString(std::to_string(*_value));

	_handle->SetPos({ pos.x + _handleoffset.x, pos.y, 0.0f });


	//ドラッグフラグの処理
	if (ishover && Input::GetMousePress(MOUSE_BUTTON::MOUSE_LBUTTON)) {
		_isdrag = true;
	}
	else {
		_isdrag = false;
	}
}

//描画処理
void Slidebar::Draw() {
	GUI::Draw();
	if (_handle) {
		_handle->Draw();
	}
}

//クリックしたときは特に何もなし
bool Slidebar::OnClicked(const int& mousebutton) {
	return false;
}

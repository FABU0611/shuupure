//G_Slidebar.cpp
//20106_�c���@�@
//25_02_26
#include "Text.h"
#include "G_Slidebar.h"
#include "C_Sprite2D.h"
#include "Input.h"
#include "Manager.h"
#include "GUIManager.h"
#include "TextManager.h"


const XMFLOAT3 Slidebar::_slidersize = { 200.0f, 25.0f, 0.0f };	//�X���C�_�[�o�[�̃T�C�Y

//�R���X�g���N�^
Slidebar::Slidebar(const std::string& label, const XMFLOAT3& pos, const float& max, const float& min, float* value)
:GUI(pos, false, _slidersize, L"asset\\texture\\slidebar_bar.png"), _max(max), _min(min), _value(value){
	//�l���ő�l�ƍŏ��l�̊ԂɎ��߂�
	*_value = std::max(_min, std::min(_max, *_value));

	//�l���E���ɕ\��
	_valuetext = TextManager::GetInstance()->AddText<Text>(_size.y, D2D1::ColorF::Black, TextAnchor::CenterLeft);
	_valuetext->SetString(std::to_string(*_value));
	_valuetext->SetPosition({ pos.x + _size.x + _slidersize.y, pos.y + _size.y * 0.5f, 0.0f });

	//���x���������ɕ\��
	_labeltext = TextManager::GetInstance()->AddText<Text>(_size.y, D2D1::ColorF::Black, TextAnchor::TopLeft);
	_labeltext->SetString(label);
	_labeltext->SetPosition({ pos.x, pos.y - _size.y, 0.0f });
}

//����������
void Slidebar::Init() {
	GUI::Init();

	//�X���C�_�[�n���h���̃e�N�X�`����ǂݍ���
	_handle = AddComponent<Sprite2D>(this);
	_handle->LoadTexture(L"asset\\texture\\slidebar_handle.png");
	_handle->SetDrawMode(DrawMode::Color);
	_handle->SetSize({ _size.y, _size.y, 0.0f });
}

//�X�V����
void Slidebar::Update() {
	POINT mousepos = Input::GetMousePosition();
	bool ishover = IsHovered(mousepos);


	//�}�E�X�z�C�[���Œl��ύX
	if (ishover) {
		*_value += Input::GetMouseWheelDelta() * 0.01f;
	}

	//�h���b�O�Œl��ύX
	XMFLOAT3 pos = GetPosition();
	if (_isdrag) {
		float left = pos.x;
		float right = pos.x + _size.x;
		float x = std::max(left, std::min((float)mousepos.x, right));

		*_value = _min + (_max - _min) * ((x - left) / (_size.x));
	}
	*_value = std::max(_min, std::min(_max, *_value));

	//�l����n���h���ʒu���v�Z
	float normalized = (*_value - _min) / (_max - _min);
	_handleoffset.x = pos.x + _slidersize.x * normalized;
	_handleoffset.y = pos.y + _slidersize.y * 0.5f;
	_valuetext->SetString(std::to_string(*_value));

	_handle->SetPos(_handleoffset);


	//�h���b�O�t���O�̏���
	if (ishover && Input::GetMousePress(MOUSE_BUTTON::MOUSE_LBUTTON)) {
		_isdrag = true;
	}
	else {
		_isdrag = false;
	}
}

//�`�揈��
void Slidebar::Draw() {
	GUI::Draw();
	if (_handle) {
		_handle->Draw();
	}
}

//�N���b�N�����Ƃ��͓��ɉ����Ȃ�
bool Slidebar::OnClicked(const MOUSE_BUTTON& mousebutton) {
	return false;
}

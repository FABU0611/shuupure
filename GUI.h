//GUI.h
//20106_�c���@�@
//24_11_14
#pragma once

#include "GameObject.h"

class GUI : public GameObject{
protected:
	XMFLOAT3			_size{};
	const wchar_t*		_texname{};
	bool				_iscenter = false;	

public:
	GUI() {};
	GUI(const XMFLOAT3& pos, const bool& iscenter, const XMFLOAT3& size, const wchar_t* texname);
	virtual ~GUI() {}

	virtual void Init();
	virtual void Uninit();
	virtual void Update() override{}
	virtual void Draw();

	virtual bool IsHovered(const POINT pos);	//�}�E�X���d�Ȃ��Ă��邩
	virtual bool OnClicked(const int& mousebutton) = 0;
};
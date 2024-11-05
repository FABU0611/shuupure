//Title.h
//20106_ìcíÜÅ@ò@
//24_09_15
#pragma once
#include "GameObject.h"
#include "DirectWriteCustomFont.h"

class TitleText : public GameObject {
	FontData* _fontdata;
	DirectWriteCustomFont* _title;

	XMFLOAT2	_spos;
	XMFLOAT2	_epos;
	XMFLOAT2	_drawpos;

public:
	TitleText() {}
	~TitleText() {
		delete _title;
		delete _fontdata;
	}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
//TitlePush.h
//20106_ìcíÜÅ@ò@
//24_09_15
#pragma once
#include "GameObject.h"
#include "DirectWriteCustomFont.h"

class TitlePush : public GameObject {
	FontData* _fontdata;
	DirectWriteCustomFont* _push;

	XMFLOAT2	_spos;
	XMFLOAT2	_epos;
	XMFLOAT2	_drawpos;
	float	_pos;

public:
	TitlePush() {}
	~TitlePush() {
		delete _push;
		delete _fontdata;
	}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
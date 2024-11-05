//TutorialText.h
//20106_ìcíÜÅ@ò@
//24_09_17
#pragma once
#include "GameObject.h"
#include "DirectWriteCustomFont.h"

class TutorialText : public GameObject {
	FontData* _fontdata;
	DirectWriteCustomFont* _tutoT;

	XMFLOAT2	_spos;
	XMFLOAT2	_epos;
	XMFLOAT2	_drawpos;
	float	_pos;

public:
	TutorialText() {}
	~TutorialText() {
		delete _tutoT;
		delete _fontdata;
	}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
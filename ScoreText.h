//ScoreText.h
//20106_ìcíÜÅ@ò@
//24_09_15
#pragma once
#include "GameObject.h"
#include "DirectWriteCustomFont.h"

class ScoreText : public GameObject {
	FontData* _fontdata;
	DirectWriteCustomFont* _scoreT;
	DirectWriteCustomFont* _dotT;
	DirectWriteCustomFont* _backT;

	XMFLOAT2	_spos;
	XMFLOAT2	_epos;
	XMFLOAT2	_drawpos;
	float	_pos;
	float	_time;

	float _score;
	float _curscore;
	float _dot;
	int _twist;
	int _rot;
	float _alpha;

public:
	ScoreText() {}
	~ScoreText() {
		delete _backT;
		delete _dotT;
		delete _scoreT;
		delete _fontdata;
	}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
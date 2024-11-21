//T_Loading.h
//20106_ìcíÜÅ@ò@
//24_11_21
#pragma once
#include "GameObject.h"
#include "DirectWriteCustomFont.h"

class LoadingText : public GameObject {
	FontData*				_fontdata{};
	DirectWriteCustomFont*	_loadT{};

	std::string				_loading{};
	float					_time{};
	float					_fadealpha{};

	XMFLOAT2				_spos{};
	XMFLOAT2				_epos{};
	XMFLOAT2				_drawpos{};
	float					_pos{};

public:
	LoadingText() {}
	~LoadingText() {
		delete _loadT;
		delete _fontdata;
	}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetFadealpha(const float& alpha) { _fadealpha = alpha; }
};
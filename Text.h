//Text.h
//20106_“c’†@˜@
//24_11_26
#pragma once

#include "GameObject.h"
#include "DirectWriteCustomFont.h"

class Text : public GameObject {
protected:
	FontData				_fontdata{};
	DirectWriteCustomFont*	_text{};

	std::string				_str{};

	float					_size = 30.0f;
	D2D1::ColorF			_color = D2D1::ColorF::Black;

	TextAnchor				_anchor = TextAnchor::TopLeft;


	void SetColor(const D2D1::ColorF& color) {
		if (!_text) {
			return;
		}
		_fontdata.Color = color;
		_text->SetFont(_fontdata);
	}
	void SetSize(const float& size) {
		if (!_text) {
			return;
		}
		_fontdata.fontSize = size;
		_text->SetFont(_fontdata);
	}

public:
	Text() {}
	Text(const float& size, const D2D1::ColorF& color, const TextAnchor& anchor)
		:_size(size), _color(color), _anchor(anchor) {}
	Text(const float& size, const D2D1::ColorF& color)
		:_size(size), _color(color), _anchor(TextAnchor::TopLeft) {}
	virtual ~Text() {
		Uninit();
	}

	void SetString(const std::string& str) {
		_str = str;
	}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};
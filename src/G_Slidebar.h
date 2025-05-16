//G_Slidebar.h
//20106_田中　蓮
//25_02_26
#pragma once
#include "GUI.h"

class Text;

class Slidebar : public GUI {
	static const XMFLOAT3		_slidersize;	//スライダーバーのサイズ

	const float		_max{};
	const float		_min{};
	float*			_value{};
	
	XMFLOAT3		_handleoffset{};
	class Sprite2D*	_handle = nullptr;
	Text*			_valuetext = nullptr;
	Text*			_labeltext = nullptr;

	bool			_isdrag = false;

public:
	/// <summary>
	///			
	/// </summary>
	/// <param name="label">Slider Label</param>
	/// <param name="pos">Center Position to Slider</param>
	/// <param name="max">Maximam Value</param>
	/// <param name="min">Minimam Value</param>
	/// <param name="value">Change Value Pointer</param>
	Slidebar(const std::string& label, const XMFLOAT3& pos, const float& max, const float& min, float* value);
	~Slidebar() {}

	void Init() override;
	void Update() override;
	void Draw() override;

	bool OnClicked(const MOUSE_BUTTON& mousebutton) override;
};
//C_Sprite2D.h
//20106_ìcíÜÅ@ò@
//24_07_16
#pragma once

#include "C_Sprite.h"

enum class DrawMode {
	Normal = 0,
	LeftTop,
	LeftTopColor,
	Color,
	Rotate,
	ColorRotate,

	ModeNum,
};

class Sprite2D : public Sprite {
	XMFLOAT3		_size = { 1.0f, 1.0f, 0.0f };
	DrawMode		_mode = DrawMode::Normal;

public:
	using Sprite::Sprite;
	void Draw()override;
	void SetSize(const XMFLOAT3& size) { _size = size; }
	const XMFLOAT3& GetSize() { return _size; }
	void SetDrawMode(const DrawMode& mode) { _mode = mode; }
};
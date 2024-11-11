//C_Srite3D.h	3Dテクスチャ表示
//20106_田中　蓮
//24_07_17
#pragma once

enum class DrawMode3D {
	Normal = 0,
	Color,
	Billboard,
	BillboardColor,

	ModeNum,
};

#include "C_Sprite.h"
/// <summary>
/// 必ずSetRotをしてください
/// </summary>
class Sprite3D : public Sprite {
	DrawMode3D		_mode = DrawMode3D::Normal;
	XMFLOAT3		_scl{};

	XMMATRIX			_prevworld;

public:
	using Sprite::Sprite;
	void Draw()override;
	void SetDrawMode(const DrawMode3D& mode) { _mode = mode; }
	void SetScale(const XMFLOAT3& scl) { _scl = scl; }
};
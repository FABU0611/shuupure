//C_Sprite2D.h
//20106_田中　蓮
//24_07_16
#pragma once

#include "C_Sprite.h"
#include <list>

enum class DrawMode {
	Normal = 0,		//中心
	LeftTop,		//左上
	LeftTopColor,	//左上色変更可能
	Color,			//色変更可能
	Rotate,			//回転
	RotateColor,	//回転色変更可能
	Multiply,		//マルチ

	ModeNum,
};

class Sprite2D : public Sprite {
	XMFLOAT3		_size = { 1.0f, 1.0f, 0.0f };
	DrawMode		_mode = DrawMode::Normal;
	std::list<ID3D11ShaderResourceView*> _srv;

public:
	using Sprite::Sprite;
	void Draw()override;
	void Uninit()override;
	void SetSize(const XMFLOAT3& size) { _size = size; }
	const XMFLOAT3& GetSize() { return _size; }
	void SetDrawMode(const DrawMode& mode) { _mode = mode; }
	void SetDispSRV(const std::list<ID3D11ShaderResourceView*>& srv) { _srv = srv; }
};
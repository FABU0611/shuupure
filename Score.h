//Score.h
//20106_田中　蓮
//2024_07_16
#pragma once

#include "GameObject.h"

class Score : public GameObject{
	ID3D11VertexShader*			_VertexShader;
	ID3D11PixelShader*			_PixelShader;
	ID3D11InputLayout*			_VertexLayout;

	const wchar_t*				_texname;		//画像ファイルのパス

	int							_score = 0;
	XMFLOAT2					_uv;
	int							_anim_count = 0;
	const int					_anime_ptn_x = 5;	//横
	const int					_anime_ptn_y = 5;	//縦
	const int					_anime_ptn = _anime_ptn_x * _anime_ptn_y;
	float						_ptn_width;		//エフェクト一つ分の横幅
	float						_ptn_height;	//エフェクト一つ分の縦幅

	int							_rot;
	int							_twist;
	float						_dot;

public:
	Score(const XMFLOAT3& pos, const XMFLOAT3& size);
	~Score() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetScore(const int& score) { _score = score; }
	int GetScore()const { return _score; }
	const void AddScore(const int& score) { _score += score; }
};
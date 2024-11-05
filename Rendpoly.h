//Rendpoyl.h
//20106_田中　蓮
//24_09_26

#pragma once


class Rendpoly{
	ID3D11Buffer*				_VertexBuffer = NULL;

	//ここに	シェーダー関連の変数を追加
	ID3D11VertexShader*			_VertexShader;	//頂点シェーダーオブジェクトポインタ
	ID3D11PixelShader*			_PixelShader;	//ピクセルシェーダーオブジェクトポインタ
	ID3D11InputLayout*			_VertexLayout;	//頂点レイアウトオブジェクト

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
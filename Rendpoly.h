//Rendpoyl.h
//20106_田中　蓮
//24_09_26

#pragma once


class Rendpoly{
	ID3D11Buffer*				_vertexbuffer = NULL;

	//ここに	シェーダー関連の変数を追加
	ID3D11VertexShader*			_vertexshader;	//頂点シェーダーオブジェクトポインタ
	ID3D11PixelShader*			_pixelshader;	//ピクセルシェーダーオブジェクトポインタ
	ID3D11InputLayout*			_vertexlayout;	//頂点レイアウトオブジェクト

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
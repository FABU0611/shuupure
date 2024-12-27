//C_Srite.h		テクスチャ表示
//20106_田中　蓮
//24_07_17
#pragma once

#include "Component.h"


class Sprite : public Component {
	void CheckFileextension(const wchar_t*& filename, ID3D11ShaderResourceView*& texture);

protected:
	ID3D11Buffer* _vertexbuffer = NULL;
	ID3D11ShaderResourceView* _texture = NULL;
	ID3D11ShaderResourceView* _normaltexture = NULL;

	static const int MAX_TEXTURE = 100;

	XMFLOAT3		_pos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3		_rot = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3		_normal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT2		_uv = { 0.0f, 0.0f };
	XMFLOAT2		_uvend = { 1.0f, 1.0f };
	XMFLOAT4		_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT3		_tangent = { 1.0f, 0.0f, 0.0f };

	static int				_textureindex;
	static const wchar_t* _texname[MAX_TEXTURE];		//画像ファイルのパス
	static ID3D11ShaderResourceView* _textures[MAX_TEXTURE];		//画像

	void CreateVertexBuffer();

public:
	using Component::Component;
	void LoadTexture(const wchar_t* filename);
	void LoadNormalTexture(const wchar_t* filename);
	void Uninit()override;
	void Draw()override {}
	static void UninitAll();

	void SetPos(const XMFLOAT3& pos) { _pos = pos; }
	/// <summary>
	/// 2Dの場合はzに入力
	/// </summary>
	/// <param name="rot"></param>
	void SetRot(const XMFLOAT3& rot) { _rot = rot; }
	void SetUV(const XMFLOAT2& uv) { _uv = uv; }
	void SetUVend(const XMFLOAT2& uvend) { _uvend = uvend; }
	void SetColor(const XMFLOAT4& color) { _color = color; }
	void SetNormal(const XMFLOAT3& normal) { _normal = normal; }
	void SetTangent(const XMFLOAT3& tangent) { _tangent = tangent; }
	void SetTexInfo(const XMFLOAT3& pos, const XMFLOAT2& uv, const XMFLOAT2& uvend, const XMFLOAT4& color) {
		_pos = pos;
		_uv = uv;
		_uvend = uvend;
		_color = color;
	}
};
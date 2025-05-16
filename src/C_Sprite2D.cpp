//C_Sprite2D.cpp
//20106_田中　蓮
//24_07_16
#include "Main.h"
#include "Renderer.h"
#include "C_Sprite2D.h"


void Sprite2D::Draw(){
	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(_vertexbuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//頂点設定---------------------------------------------------------------------------
	if (_mode == DrawMode::LeftTop ||
		_mode == DrawMode::LeftTopColor) {
		vertex[0].Position = { _pos.x, _pos.y, 0.0f };
		vertex[1].Position = { _pos.x + _size.x, _pos.y, 0.0f };
		vertex[2].Position = { _pos.x, _pos.y + _size.y, 0.0f };
		vertex[3].Position = { _pos.x + _size.x, _pos.y + _size.y, 0.0f };
	}
	else if (_mode == DrawMode::Rotate ||
		_mode == DrawMode::RotateColor) {
		float hw, hh;
		hw = _size.x * 0.5f;
		hh = _size.y * 0.5f;

		//頂点位置設定
		vertex[0].Position = { _pos.x - hw, _pos.y - hh, 0.0f };
		vertex[1].Position = { _pos.x + hw, _pos.y - hh, 0.0f };
		vertex[2].Position = { _pos.x - hw, _pos.y + hh, 0.0f };
		vertex[3].Position = { _pos.x + hw, _pos.y + hh, 0.0f };

		//Z軸周りの回転行列を作成
		XMMATRIX rotationMatrixZ = XMMatrixRotationZ(_rot.z);

		// 総合的な回転行列を計算
		XMMATRIX rotationMatrix = rotationMatrixZ;

		for (int i = 0; i < 4; i++) {
			// 元の頂点座標を中心に移動
			XMVECTOR originalPos = XMVectorSet(vertex[i].Position.x - _pos.x, vertex[i].Position.y - _pos.y, vertex[i].Position.z - 0.0f, 1.0f);

			// 回転行列を適用して新しい座標を計算
			XMVECTOR rotatedPos = XMVector3Transform(originalPos, rotationMatrix);

			// 中心座標を再度加算して、新しい座標を設定
			rotatedPos = XMVectorAdd(rotatedPos, XMVectorSet(_pos.x, _pos.y, 0.0f, 0.0f));
			XMStoreFloat3(&vertex[i].Position, rotatedPos);
			vertex[i].Normal = { 0.0f, 0.0f, 0.0f };
			vertex[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}
	else if (_mode == DrawMode::Multiply) {
		for (int i = 0; i < _dispNum; i++) {
			vertex[i * 4 + 0].Position = { _pos.x + _size.x * (0.25f * i) + (10.0f * i), _pos.y, 0.0f };
			vertex[i * 4 + 1].Position = { _pos.x + _size.x * (0.25f * (i + 1)) + (10.0f * i), _pos.y, 0.0f };
			vertex[i * 4 + 2].Position = { _pos.x + _size.x * (0.25f * i) + (10.0f * i), _pos.y + _size.y * 0.25f, 0.0f };
			vertex[i * 4 + 3].Position = { _pos.x + _size.x * (0.25f * (i + 1)) + (10.0f * i), _pos.y + _size.y * 0.25f, 0.0f };
		}
	}
	else {
		float hw, hh;
		hw = _size.x * 0.5f;
		hh = _size.y * 0.5f;

		vertex[0].Position = { _pos.x - hw, _pos.y - hh, 0.0f };
		vertex[1].Position = { _pos.x + hw, _pos.y - hh, 0.0f };
		vertex[2].Position = { _pos.x - hw, _pos.y + hh, 0.0f };
		vertex[3].Position = { _pos.x + hw, _pos.y + hh, 0.0f };
	}

	//法線設定---------------------------------------------------------------------------
	for (int i = 0; i < 4 * _dispNum; i++) {
		vertex[i].Normal = { 0.0f, 0.0f, -1.0f };
	}

	//色設定-----------------------------------------------------------------------------
	if (_mode == DrawMode::Color ||
		_mode == DrawMode::LeftTopColor ||
		_mode == DrawMode::RotateColor) {
		vertex[0].Diffuse = _color;
		vertex[1].Diffuse = _color;
		vertex[2].Diffuse = _color;
		vertex[3].Diffuse = _color;
	}
	else {
		for (int i = 0; i < 4 * _dispNum; i++) {
			vertex[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}

	//UV設定-----------------------------------------------------------------------------
	if (_mode == DrawMode::Multiply) {
		for (int i = 0; i < _dispNum; i++) {
			vertex[i * 4 + 0].TexCoord = { 0.0f, 0.0f };
			vertex[i * 4 + 1].TexCoord = { 1.0f, 0.0f };
			vertex[i * 4 + 2].TexCoord = { 0.0f, 1.0f };
			vertex[i * 4 + 3].TexCoord = { 1.0f, 1.0f };
		}
	}
	else{
		vertex[0].TexCoord = { _uv.x, _uv.y };
		vertex[1].TexCoord = { _uv.x + _uvend.x, _uv.y };
		vertex[2].TexCoord = { _uv.x, _uv.y + _uvend.y };
		vertex[3].TexCoord = { _uv.x + _uvend.x, _uv.y + _uvend.y };
	}

	Renderer::GetDeviceContext()->Unmap(_vertexbuffer, 0);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (_mode == DrawMode::Multiply) {
		//テクスチャ設定
		int i = 0;
		for (const auto& srv : _srv) {
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			Renderer::GetDeviceContext()->Draw(4, 4 * i);
			i++;
		}
	}
	else {
		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
	_srv.clear();
}

void Sprite2D::Uninit() {
	Sprite::Uninit();

	_srv.clear();
}

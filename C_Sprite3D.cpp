//C_Srite3D.cpp	3Dテクスチャ表示
//20106_田中　蓮
//24_07_17
#include "Main.h"
#include "Renderer.h"
#include "Scene.h"
#include "Manager.h"
#include "ParticleEmitter.h"
#include "C_Sprite3D.h"

void Sprite3D::Draw() {
	_scl = _gameobject->GetScale();
	_pos = _gameobject->GetPosition();

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(_vertexbuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//頂点設定---------------------------------------------------------------------------
	vertex[0].Position = { -_scl.x,  _scl.y, 0.0f };
	vertex[1].Position = { _scl.x,  _scl.y, 0.0f };
	vertex[2].Position = { -_scl.x, -_scl.y, 0.0f };
	vertex[3].Position = { _scl.x, -_scl.y, 0.0f };

	for (int i = 0; i < 4; i++) {
		vertex[i].Tangent = _tangent;
	}

	if (_mode == DrawMode3D::Normal) {
		_rot = _gameobject->GetRotation();

		// 総合的な回転行列を計算
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(_rot.x), XMConvertToRadians(_rot.y), XMConvertToRadians(_rot.z));

		XMVECTOR normal = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

		for (int i = 0; i < 4; i++) {
			vertex[i].Normal = _normal;

			// 元の頂点座標を中心に移動
			XMVECTOR originalPos = XMVectorSet(vertex[i].Position.x, vertex[i].Position.y, vertex[i].Position.z, 1.0f);

			// 新しい位置を計算
			XMVECTOR rotatedPosition = XMVector3TransformCoord(originalPos, rotationMatrix);
			rotatedPosition += XMLoadFloat3(&_pos);

			// 計算結果を設定
			XMStoreFloat3(&vertex[i].Position, rotatedPosition);


			XMVECTOR rotatedNormal = XMVector3TransformNormal(normal, rotationMatrix);
			XMStoreFloat3(&vertex[i].Normal, rotatedNormal);
		}

		//ワールドマトリクス設定
		XMMATRIX world, scale, rot, trans;
		scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		trans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(world, _prevworld);
	}
	else if (_mode == DrawMode3D::Billboard ||
		_mode == DrawMode3D::BillboardColor) {
		//カメラのビューマトリクス取得
		Scene* scene = Manager::GetScene();
		Camera* camera = scene->GetGameobject<Camera>();
		XMMATRIX view = camera->GetViewMatrix();

		//ビューの逆行列
		XMMATRIX invView = XMMatrixInverse(nullptr, view);
		invView.r[3].m128_f32[0] = 0.0f;
		invView.r[3].m128_f32[1] = 0.0f;
		invView.r[3].m128_f32[2] = 0.0f;

		//ワールドマトリクス設定
		XMMATRIX world, scale, trans;
		scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		trans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(world, _prevworld);
	}

	//色設定-----------------------------------------------------------------------------
	if (_mode == DrawMode3D::Color ||
		_mode == DrawMode3D::BillboardColor) {
		vertex[0].Diffuse = _color;
		vertex[1].Diffuse = _color;
		vertex[2].Diffuse = _color;
		vertex[3].Diffuse = _color;
	}
	else {
		vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	//UV設定-----------------------------------------------------------------------------
	vertex[0].TexCoord = { _uv.x, _uv.y };
	vertex[1].TexCoord = { _uv.x + _uvend.x, _uv.y };
	vertex[2].TexCoord = { _uv.x, _uv.y + _uvend.y };
	vertex[3].TexCoord = { _uv.x + _uvend.x, _uv.y + _uvend.y };


	Renderer::GetDeviceContext()->Unmap(_vertexbuffer, 0);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);
	if (_normaltexture != NULL) {
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &_normaltexture);
	}
	if (_envtexture != NULL) {
		Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &_envtexture);
	}

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (_mode == DrawMode3D::Particle) {
		//カメラのビューマトリクス取得
		Scene* scene = Manager::GetScene();
		Camera* camera = scene->GetGameobject<Camera>();
		XMMATRIX view = camera->GetViewMatrix();

		//ビューの逆行列
		XMMATRIX invView = XMMatrixInverse(nullptr, view);
		invView.r[3].m128_f32[0] = 0.0f;
		invView.r[3].m128_f32[1] = 0.0f;
		invView.r[3].m128_f32[2] = 0.0f;

		//ワールドマトリクス設定
		XMMATRIX world, scale, trans;
		scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		trans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(world, _prevworld);

		//Zバッファ無効
		Renderer::SetDepthEnable(false);	//パーティクルをZソートするのは負荷が高い
		//加算合成に変更
		Renderer::SetBlendAddEnable(true);

		//パーティクル表示
		ParticleEmitter* emitter = dynamic_cast<ParticleEmitter*>(_gameobject);
		if (!emitter) {
			_mode = DrawMode3D::Normal;
			return;
		}
		Renderer::GetDeviceContext()->DrawInstanced(4, emitter->_instancecount, 0, 0);
		
		//加算合成を無効
		Renderer::SetBlendAddEnable(false);
		//Zバッファ有効
		Renderer::SetDepthEnable(true);
		return;
	}
	else {
		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);
	}
	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

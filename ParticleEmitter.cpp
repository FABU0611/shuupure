//ParticleEmitter.cpp
//20106_田中　蓮
//2024_07_05
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "ParticleEmitter.h"
#include "Scene.h"
#include <random>
#include "ParticleFireworks.h"

//初期化処理
void ParticleEmitter::Init() {
	VERTEX_3D vertex[4];

	for (int i = 0; i < 4; i++) {
		vertex[i].Normal = { 0.0f, 0.0f, 0.0f };
		vertex[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	vertex[0].Position = { -_size, _size, 0.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { _size, _size, 0.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { -_size, -_size, 0.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { _size, -_size, 0.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);


	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(_texname, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_texture);
	assert(_texture);


	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");
}

//終了処理
void ParticleEmitter::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexbuffer->Release();
	_texture->Release();

	_VertexShader->Release();
	_PixelShader->Release();
	_VertexLayout->Release();
}

//更新処理
void ParticleEmitter::Update() {}

//描画処理
void ParticleEmitter::Draw() {
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	//カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameobject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();

	//ビューの逆行列
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);	//逆行列
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);


	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Zバッファ無効
	Renderer::SetDepthEnable(false);	//パーティクルをZソートするのは負荷が高い
	//加算合成に変更
	Renderer::SetBlendAddEnable(true);

	//パーティクル表示
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (!_particle[i].Enable) {
			continue;
		}
		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = _particle[i].Color;
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		//ワールドマトリクス設定
		XMMATRIX world, scale, trans;
		scale = XMMatrixScaling(_particle[i].Size.x, _particle[i].Size.y, _particle[i].Size.z);
		trans = XMMatrixTranslation(_particle[i].Pos.x, _particle[i].Pos.y, _particle[i].Pos.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(world);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
	//加算合成を無効
	Renderer::SetBlendAddEnable(false);
	//Zバッファ有効
	Renderer::SetDepthEnable(true);

	for (auto c : _components) {
		c->Draw();
	}
}

//Rendpoyl.cpp
//20106_田中　蓮
//24_09_26

#include "Main.h"
#include "Renderer.h"
#include "Rendpoly.h"





void Rendpoly::Init() {
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);


	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout, "shader\\UnlitTextureVS.cso");
	Renderer::CreatePixelShader(&_pixelshader, "shader\\DepthPS.cso");
}


void Rendpoly::Uninit() {
	_vertexbuffer->Release();
	//_texture->Release();


	//ここにシェーダーオブジェクトの解放を追加
	_vertexshader->Release();
	_vertexlayout->Release();
	_pixelshader->Release();
}


void Rendpoly::Update() {}


void Rendpoly::Draw() {
	//ここにシェーダー関連の描画準備を追加

	//頂点レイアウトを設定
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);
	//頂点シェーダーをセット
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	//ピクセルシェーダーをセット
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);


	// マトリクス設定
	Renderer::SetWorldViewProjection2D();


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	ID3D11ShaderResourceView* ppTexture = Renderer::GetMBTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &ppTexture);
	ID3D11ShaderResourceView* bsTexture = Renderer::GetBYTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &bsTexture);
	ID3D11ShaderResourceView* dsTexture = Renderer::GetDepthTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &dsTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
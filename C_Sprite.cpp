//C_Srite.cpp		テクスチャ表示
//20106_田中　蓮
//24_07_17
#include "Main.h"
#include "Renderer.h"
#include "C_Sprite.h"
#include "ErrorHandler.h"
#include "TextureManager.h"
#include <string>
#include <cwchar>


void Sprite::CreateVertexBuffer(const unsigned short& num){
	if (_vertexbuffer != NULL) {
		return;
	}
	_dispNum = num;
	int vertexNum = 4 * num;
	VERTEX_3D* vertex = new VERTEX_3D[vertexNum];

	for (int i = 0; i < (4 * num); i++) {
		vertex[i].Position = { 0.0f, 0.0f, 0.0f };
		vertex[i].Normal = { 0.0f, 1.0f, 0.0f };
		vertex[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[i].Tangent = _tangent;
	}
	vertex[0].TexCoord = { 0.0f, 0.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	delete[] vertex;

	HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);
	ErrorHandler::GetInstance()->DispErrorMessageBox("200", hr);
}

void Sprite::LoadTexture(const wchar_t* filename) {
	CreateVertexBuffer(1);
	_texture = TextureManager::GetInstance()->LoadTexture(filename);
}


void Sprite::LoadNormalTexture(const wchar_t* filename){
	CreateVertexBuffer(1);
	_normaltexture = TextureManager::GetInstance()->LoadTexture(filename);
}

void Sprite::LoadEnvTexture(const wchar_t* filename) {
	CreateVertexBuffer(1);
	_envtexture = TextureManager::GetInstance()->LoadTexture(filename);
}


void Sprite::Uninit() {
	if (_vertexbuffer != NULL) {
		_vertexbuffer->Release();
		_vertexbuffer = NULL;
	}
}

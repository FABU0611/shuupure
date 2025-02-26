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


void Sprite::CreateVertexBuffer(){
	if (_vertexbuffer != NULL) {
		return;
	}
	VERTEX_3D vertex[4];

	for (int i = 0; i < 4; i++) {
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
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);
	ErrorHandler::GetInstance()->DispErrorMessageBox("200", hr);
}

void Sprite::LoadTexture(const wchar_t* filename) {
	CreateVertexBuffer();
	_texture = TextureManager::GetInstance()->LoadTexture(filename);
	/*

	for (int i = 0; i < _textureindex; i++) {
		if (_texname[i] == filename) {
			_texture = _textures[i];
			return;
		}
	}

	HRESULT hr = S_OK;
	TexMetadata metadata;
	ScratchImage image;
	ErrorHandler::GetInstance()->LoadTex(filename, metadata, image);
	hr = CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_texture);
	if (FAILED(hr)) {		//SRV作成失敗
		//DispErrorMessageBox(300);
		return;
	}
	assert(_texture);


	_texname[_textureindex] = filename;
	_textures[_textureindex] = _texture;
	_textureindex++;
	*/
}


void Sprite::LoadNormalTexture(const wchar_t* filename){
	CreateVertexBuffer();
	_normaltexture = TextureManager::GetInstance()->LoadTexture(filename);
	/*
	for (int i = 0; i < _textureindex; i++) {
		if (_texname[i] == filename) {
			_normaltexture = _textures[i];
			return;
		}
	}

	HRESULT hr = S_OK;
	TexMetadata metadata;
	ScratchImage image;
	ErrorHandler::GetInstance()->LoadTex(filename, metadata, image);
	hr = CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_normaltexture);
	if (FAILED(hr)) {		//SRV作成失敗
		//DispErrorMessageBox(300);
		return;
	}
	assert(_normaltexture);

	_texname[_textureindex] = filename;
	_textures[_textureindex] = _normaltexture;
	_textureindex++;
	*/
}

void Sprite::LoadEnvTexture(const wchar_t* filename) {
	CreateVertexBuffer();
	_envtexture = TextureManager::GetInstance()->LoadTexture(filename);
}


void Sprite::Uninit() {
	if (_vertexbuffer != NULL) {
		_vertexbuffer->Release();
		_vertexbuffer = NULL;
	}
}

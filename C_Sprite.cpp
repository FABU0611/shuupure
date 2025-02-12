//C_Srite.cpp		テクスチャ表示
//20106_田中　蓮
//24_07_17
#include "Main.h"
#include "Renderer.h"
#include "C_Sprite.h"
#include "ErrorHandler.h"
#include <string>
#include <cwchar>

int Sprite::_textureindex = 0;
const wchar_t* Sprite::_texname[MAX_TEXTURE]{};
ID3D11ShaderResourceView* Sprite::_textures[MAX_TEXTURE]{};


void Sprite::CheckFileextension(const wchar_t*& filename, ID3D11ShaderResourceView*& texture) {
	HRESULT hr = S_OK;

	std::wstring file(filename);
	size_t pos = file.rfind(L'.');

	std::wstring extension = file.substr(pos);

	//拡張子を調べてテクスチャ読み込み
	if (extension == L".png" || extension == L".jpg") {
		TexMetadata metadata;
		ScratchImage image;
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		if (FAILED(hr)) {		//png jpg画像のロード失敗
			DispErrorMessageBox(100);
			return;
		}
		hr = CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		if (FAILED(hr)) {		//SRV作成失敗
			DispErrorMessageBox(300);
			return;
		}
		assert(texture);
	}
	else if (extension == L".dds") {
		TexMetadata metadata;
		ScratchImage image;
		hr = LoadFromDDSFile(filename, DDS_FLAGS_NONE, &metadata, image);
		if (FAILED(hr)) {		//dds画像のロード失敗
			DispErrorMessageBox(101);
			return;
		}
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		if (FAILED(hr)) {		//SRV作成失敗
			DispErrorMessageBox(300);
			return;
		}
		assert(texture);
	}
}

void Sprite::CreateVertexBuffer(){
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
	if (FAILED(hr)) {		//頂点バッファ生成失敗
		DispErrorMessageBox(200);
		return;
	}
}

void Sprite::LoadTexture(const wchar_t* filename) {
	CreateVertexBuffer();

	for (int i = 0; i < _textureindex; i++) {
		if (_texname[i] == filename) {
			_texture = _textures[i];
			return;
		}
	}

	CheckFileextension(filename, _texture);


	_texname[_textureindex] = filename;
	_textures[_textureindex] = _texture;
	_textureindex++;
}


void Sprite::LoadNormalTexture(const wchar_t* filename){
	CreateVertexBuffer();

	for (int i = 0; i < _textureindex; i++) {
		if (_texname[i] == filename) {
			_normaltexture = _textures[i];
			return;
		}
	}

	CheckFileextension(filename, _normaltexture);

	_texname[_textureindex] = filename;
	_textures[_textureindex] = _normaltexture;
	_textureindex++;
}


void Sprite::Uninit() {
	if (_vertexbuffer != NULL) {
		_vertexbuffer->Release();
	}
}

void Sprite::UninitAll(){
	for (int i = 0; i < MAX_TEXTURE; i++) {
		_texname[i] = {};
		if (_textures[i] != NULL) {
			_textures[i]->Release();
		}
		_textures[i] = {};
	}
	_textureindex = 0;
}

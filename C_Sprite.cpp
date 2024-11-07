//C_Srite.cpp		テクスチャ表示
//20106_田中　蓮
//24_07_17
#include "Main.h"
#include "Renderer.h"
#include "C_Sprite.h"

int Sprite::_textureindex = 0;
const wchar_t* Sprite::_texname[MAX_TEXTURE]{};
ID3D11ShaderResourceView* Sprite::_textures[MAX_TEXTURE]{};

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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);
}

void Sprite::LoadTexture(const wchar_t* filename) {
	CreateVertexBuffer();

	//読みこむファイルがすでに読みこんでいるかを調べる
	//Drawで_textureを使っているんだからそれを保存しておかないとだめ
	for (int i = 0; i < _textureindex; i++) {
		if (_texname[i] == filename) {
			_texture = _textures[i];
			return;
		}
	}

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_texture);
	assert(_texture);

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

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_normaltexture);
	assert(_normaltexture);

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

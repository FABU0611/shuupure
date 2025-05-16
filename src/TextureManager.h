//TextureManager.h
//20106_田中　蓮
//25_02_17
#pragma once
#include <map>
#include "Main.h"

class TextureManager {
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	static TextureManager* _instance;
	//シングルトンの解放
	static void DeleteInstance();

	std::map<const wchar_t*, ID3D11ShaderResourceView*>	_textures;
	void ImageFormat(const wchar_t* filename, TexMetadata& metadata, ScratchImage& image);
	void Uninit();

	friend class Manager;

public:
	//シングルトンの取得
	static TextureManager* GetInstance();
	ID3D11ShaderResourceView* LoadTexture(const wchar_t* filename);
};
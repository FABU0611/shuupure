//TextureManager.cpp
//20106_“c’†@˜@
//25_02_17
#include "TextureManager.h"
#include "ErrorHandler.h"
#include "Renderer.h"

TextureManager* TextureManager::_instance = nullptr;

void TextureManager::DeleteInstance() {
	if (_instance) {
		_instance->Uninit();
		delete _instance;
		_instance = nullptr;
	}
}

void TextureManager::ImageFormat(const wchar_t* filename, TexMetadata& metadata, ScratchImage& image) {
	const wchar_t* extension = wcsrchr(filename, L'.');

	if (!extension) {
		return ErrorHandler::GetInstance()->DispErrorMessageBox(108);
	}

	HRESULT hr = E_FAIL;

	if (wcscmp(extension, L".png") == 0) {
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(100, hr);
	}
	else if (wcscmp(extension, L".jpg") == 0) {
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(101, hr);
	}
	else if (wcscmp(extension, L".jpeg") == 0) {
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(102, hr);
	}
	else if (wcscmp(extension, L".bmp") == 0) {
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(103, hr);
	}
	else if (wcscmp(extension, L".gif") == 0) {
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(104, hr);
	}
	else if (wcscmp(extension, L".tiff") == 0 || wcscmp(extension, L".TIFF") == 0) {
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(105, hr);
	}
	else if (wcscmp(extension, L".tif") == 0 || wcscmp(extension, L".TIF") == 0) {
		hr = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(106, hr);
	}
	else if (wcscmp(extension, L".dds") == 0) {
		hr = LoadFromDDSFile(filename, DDS_FLAGS_NONE, &metadata, image);
		ErrorHandler::GetInstance()->DispErrorMessageBox(107, hr);
	}
	else {
		ErrorHandler::GetInstance()->DispErrorMessageBox(109);
	}
}

void TextureManager::Uninit() {
	for (auto& tex : _instance->_textures) {
		if (tex.second == NULL) {
			continue;
		}
		tex.second->Release();
		tex.second = nullptr;
	}
	_instance->_textures.clear();
}

TextureManager* TextureManager::GetInstance() {
	if (!_instance) {
		_instance = new TextureManager();
	}
	return _instance;
}

ID3D11ShaderResourceView* TextureManager::LoadTexture(const wchar_t* filename) {
	auto it = _instance->_textures.find(filename);
	if (it != _instance->_textures.end()) {
		return it->second;
	}

	ID3D11ShaderResourceView* texture = nullptr;
	HRESULT hr = E_FAIL;

	TexMetadata metadata;
	ScratchImage image;
	_instance->ImageFormat(filename, metadata, image);
	hr = CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
	assert(texture);
	ErrorHandler::GetInstance()->DispErrorMessageBox(300, hr);

	_instance->_textures[filename] = texture;

	return _instance->_textures[filename];
}

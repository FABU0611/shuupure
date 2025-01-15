//TakePicture.cpp
//20106_�c���@�@
//25_01_13
#include "TakePicture.h"
#include "Main.h"
#include "Renderer.h"
#include "Input.h"
#include "Time.h"
#include <string>
#include <sstream>
#include <filesystem>

void TakePicture::TakePic() {
	if (Input::GetKeyTrigger('T')) {
		D3D11_TEXTURE2D_DESC desc;
		ID3D11Texture2D* tex = Renderer::GetRendertargetTEX();
		ID3D11Device* device = Renderer::GetDevice();
		ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();

		tex->GetDesc(&desc);
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		ID3D11Texture2D* stagingTexture = nullptr;
		HRESULT hr = device->CreateTexture2D(&desc, nullptr, &stagingTexture);
		if (FAILED(hr)) {
			// �G���[�n���h�����O
			return;
		}

		//�����_�����O�^�[�Q�b�g����X�e�[�W���O�o�b�t�@�ɃR�s�[
		Renderer::GetDeviceContext()->CopyResource(stagingTexture, tex);

		//�X�e�[�W���O�o�b�t�@����f�[�^���擾
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = devicecontext->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
		if (FAILED(hr)) {
			stagingTexture->Release();
			return;
		}

		//DirectXTex���g�p����PNG�ɕۑ�
		Image image;
		image.width = desc.Width;
		image.height = desc.Height;
		image.format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		image.rowPitch = mappedResource.RowPitch;
		image.slicePitch = mappedResource.DepthPitch;
		image.pixels = reinterpret_cast<uint8_t*>(mappedResource.pData);

		std::wstringstream filename;
		std::wstring time = Time::GetNowRealTime();

		filename << L"photo\\" << time << L".png";

		hr = SaveToWICFile(image, WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), filename.str().c_str());
		if (hr == HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND)) {
			std::filesystem::create_directory("photo");
			SaveToWICFile(image, WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), filename.str().c_str());
		}

		devicecontext->Unmap(stagingTexture, 0);
		stagingTexture->Release();
	}
}

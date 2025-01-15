//Renderer.cpp
//20106_�c���@�@
//24_04_24
#include "Main.h"
#include "Renderer.h"
#include <io.h>
#include <vector>

#define IntelGPU (0x8086)
#define NVIDIAGPU (0x10DE)


D3D_FEATURE_LEVEL       Renderer::_featurelevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device* Renderer::_device{};
ID3D11DeviceContext* Renderer::_devicecontext{};
IDXGISwapChain* Renderer::_swapchain{};
ID3D11RenderTargetView* Renderer::_rendertargetview{};
ID3D11DepthStencilView* Renderer::_depthstencilview{};

ID3D11Buffer* Renderer::_worldbuffer{};
ID3D11Buffer* Renderer::_viewbuffer{};
ID3D11Buffer* Renderer::_projectionbuffer{};
ID3D11Buffer* Renderer::_prevworldbuffer{};
ID3D11Buffer* Renderer::_prevviewbuffer{};
ID3D11Buffer* Renderer::_prevprojectionbuffer{};
ID3D11Buffer* Renderer::_materialbuffer{};
ID3D11Buffer* Renderer::_lightbuffer{};
ID3D11Buffer* Renderer::_camerabuffer{};
ID3D11Buffer* Renderer::_parameterbuffer{};
ID3D11Buffer* Renderer::_weightsbuffer{};
ID3D11Buffer* Renderer::_dofbuffer{};
ID3D11Buffer* Renderer::_cascadesplitbuffer{};

XMMATRIX Renderer::_prevworld;
XMMATRIX Renderer::_prevview;
XMMATRIX Renderer::_prevprojection;

ID3D11DepthStencilState* Renderer::_depthstateenable{};
ID3D11DepthStencilState* Renderer::_depthstatedisable{};


ID3D11BlendState* Renderer::_blendstate{};
ID3D11BlendState* Renderer::_blendstateAdd{};
ID3D11BlendState* Renderer::_blendstateATC{};


ID3D11RenderTargetView* Renderer::_PErenderertargetview = NULL;
ID3D11ShaderResourceView* Renderer::_PEshaderresourceview = NULL;

ID3D11DepthStencilView* Renderer::_Depthstencilview = NULL;
ID3D11ShaderResourceView* Renderer::_Depthshaderresourceview = NULL;
ID3D11DepthStencilView* Renderer::_CameraDepthstencilview[3] = { NULL, NULL, NULL };
ID3D11ShaderResourceView* Renderer::_CameraDepthshaderresourceview[3] = { NULL, NULL, NULL };;
ID3D11RenderTargetView* Renderer::_BXrenderertargetview = NULL;
ID3D11ShaderResourceView* Renderer::_BXshaderresourceview = NULL;
ID3D11RenderTargetView* Renderer::_BYrenderertargetview = NULL;
ID3D11ShaderResourceView* Renderer::_BYshaderresourceview = NULL;
ID3D11RenderTargetView* Renderer::_Velrenderertargetview = NULL;
ID3D11ShaderResourceView* Renderer::_Velshaderresourceview = NULL;
ID3D11RenderTargetView* Renderer::_MBrenderertargetview = NULL;
ID3D11ShaderResourceView* Renderer::_MBshaderresourceview = NULL;

ID3D11Texture2D* Renderer::_RendertargetTEX = NULL;


void Renderer::Init() {
	HRESULT hr = S_OK;

	IDXGIAdapter* pAdapter{};
	IDXGIAdapter* pSelectAdapter{};
	IDXGIFactory* pFactory{};
	std::vector<IDXGIAdapter*> vAdapters;

	//GPU�̈ꗗ���擾���čœK�Ȃ��̂�I�񂾕�������
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	for (int i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i){	//0�Ԃ͕W����GPU
		vAdapters.push_back(pAdapter);
	}
	for (auto& adapter : vAdapters) {
		DXGI_ADAPTER_DESC desc;
		if (SUCCEEDED(adapter->GetDesc(&desc))) {
			if (desc.VendorId == IntelGPU) {
				pSelectAdapter = adapter;
				break;
			}
		}
	}
	if (pAdapter) {
		pAdapter->Release();
	}

	if (!pSelectAdapter) {
		pFactory->EnumAdapters(0, &pSelectAdapter);
	}

	for (auto& adapter : vAdapters) {
		if (adapter == pSelectAdapter) {
			continue;
		}
		adapter->Release();
	}
	if (pFactory) {
		pFactory->Release();
	}

	// �f�o�C�X�A�X���b�v�`�F�[���쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;			//MSAA�̃T���v����
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(pSelectAdapter,		//pAdapter	NULL
		D3D_DRIVER_TYPE_UNKNOWN,							//D3D_DRIVER_TYPE_UNKNOWN	D3D_DRIVER_TYPE_HARDWARE
		NULL,
		D3D10_CREATE_DEVICE_BGRA_SUPPORT,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&_swapchain,
		&_device,
		&_featurelevel,
		&_devicecontext);


	if (pSelectAdapter) {
		pSelectAdapter->Release();
	}




	// �����_�[�^�[�Q�b�g�r���[�쐬
	ID3D11Texture2D* renderTarget{};
	_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	_device->CreateRenderTargetView(renderTarget, NULL, &_rendertargetview);

	_RendertargetTEX = renderTarget;

	renderTarget->Release();


	// �f�v�X�X�e���V���o�b�t�@�쐬
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	_device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// �f�v�X�X�e���V���r���[�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//2DMS�ŃA���`�G�C���A�V���O
	depthStencilViewDesc.Flags = 0;
	_device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &_depthstencilview);
	depthStencile->Release();


	_devicecontext->OMSetRenderTargets(1, &_rendertargetview, _depthstencilview);





	//�r���[�|�[�g�ݒ�
	SetViewportSize({ (FLOAT)SCREEN_WIDTH, (FLOAT)SCREEN_HEIGHT });


	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	_device->CreateRasterizerState(&rasterizerDesc, &rs);

	_devicecontext->RSSetState(rs);




	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//�O�i�̃��ɂ����鎮(* A)
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//�w�i�̃��ɂ����鎮(1 - A)
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// �����_�[�^�[�Q�b�g1�i���x�}�b�v�j�̐ݒ�F�u�����h����
	blendDesc.RenderTarget[1].BlendEnable = FALSE;
	blendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&blendDesc, &_blendstate);


	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;				//�w�i�̃��ɂ����鎮(* 1)
	_device->CreateBlendState(&blendDesc, &_blendstateAdd);


	blendDesc.AlphaToCoverageEnable = TRUE;
	_device->CreateBlendState(&blendDesc, &_blendstateATC);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	_devicecontext->OMSetBlendState(_blendstate, blendFactor, 0xffffffff);





	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	_device->CreateDepthStencilState(&depthStencilDesc, &_depthstateenable);//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	_device->CreateDepthStencilState(&depthStencilDesc, &_depthstatedisable);//�[�x�����X�e�[�g

	_devicecontext->OMSetDepthStencilState(_depthstateenable, NULL);




	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	_device->CreateSamplerState(&samplerDesc, &samplerState);

	_devicecontext->PSSetSamplers(0, 1, &samplerState);

	//���U�C�N�p�̃T���v���[�X�e�[�g
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;	//���[�v���Ȃ�
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;	//�t�B���^�[OFF
	_device->CreateSamplerState(&samplerDesc, &samplerState);
	_devicecontext->PSSetSamplers(1, 1, &samplerState);


	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	_device->CreateBuffer(&bufferDesc, NULL, &_worldbuffer);
	_devicecontext->VSSetConstantBuffers(0, 1, &_worldbuffer);

	_device->CreateBuffer(&bufferDesc, NULL, &_viewbuffer);
	_devicecontext->VSSetConstantBuffers(1, 1, &_viewbuffer);

	_device->CreateBuffer(&bufferDesc, NULL, &_projectionbuffer);
	_devicecontext->VSSetConstantBuffers(2, 1, &_projectionbuffer);


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	_device->CreateBuffer(&bufferDesc, NULL, &_materialbuffer);
	_devicecontext->VSSetConstantBuffers(3, 1, &_materialbuffer);
	_devicecontext->PSSetConstantBuffers(3, 1, &_materialbuffer);


	bufferDesc.ByteWidth = sizeof(LIGHT);

	_device->CreateBuffer(&bufferDesc, NULL, &_lightbuffer);
	_devicecontext->VSSetConstantBuffers(4, 1, &_lightbuffer);
	_devicecontext->PSSetConstantBuffers(4, 1, &_lightbuffer);


	bufferDesc.ByteWidth = sizeof(XMFLOAT4);

	_device->CreateBuffer(&bufferDesc, NULL, &_camerabuffer);
	_devicecontext->PSSetConstantBuffers(5, 1, &_camerabuffer);


	bufferDesc.ByteWidth = sizeof(XMFLOAT4);

	_device->CreateBuffer(&bufferDesc, NULL, &_parameterbuffer);
	_devicecontext->PSSetConstantBuffers(6, 1, &_parameterbuffer);


	bufferDesc.ByteWidth = sizeof(float) * 8;

	_device->CreateBuffer(&bufferDesc, NULL, &_weightsbuffer);
	_devicecontext->PSSetConstantBuffers(7, 1, &_weightsbuffer);


	bufferDesc.ByteWidth = 16;

	_device->CreateBuffer(&bufferDesc, NULL, &_dofbuffer);
	_devicecontext->PSSetConstantBuffers(8, 1, &_dofbuffer);


	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);

	_device->CreateBuffer(&bufferDesc, NULL, &_prevworldbuffer);
	_devicecontext->VSSetConstantBuffers(9, 1, &_prevworldbuffer);

	_device->CreateBuffer(&bufferDesc, NULL, &_prevviewbuffer);
	_devicecontext->VSSetConstantBuffers(10, 1, &_prevviewbuffer);

	_device->CreateBuffer(&bufferDesc, NULL, &_prevprojectionbuffer);
	_devicecontext->VSSetConstantBuffers(11, 1, &_prevprojectionbuffer);


	bufferDesc.ByteWidth = sizeof(float) * 4;

	hr = _device->CreateBuffer(&bufferDesc, NULL, &_cascadesplitbuffer);
	_devicecontext->PSSetConstantBuffers(12, 1, &_cascadesplitbuffer);


	// ���C�g������
	LIGHT light{};
	light.Enable = true;
	light.Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	SetLight(light);



	// �}�e���A��������
	MATERIAL material{};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	SetMaterial(material);


	//09_13�ǉ�---
	//�u���[X�e�N�X�`���̍쐬
	{
		ID3D11Texture2D* bxTexture = NULL;

		D3D11_TEXTURE2D_DESC	td;		//�e�N�X�`���쐬�p�f�X�N���v�^�\����
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;

		//�쐬����~�b�v�}�b�v�̐�
		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//�s�N�Z���t�H�[�}�b�g
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//�����_�����O�^�[�Q�b�g�p�̐ݒ�
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//�e�N�X�`���̍쐬
		_device->CreateTexture2D(&td, NULL, &bxTexture);

		//�����_�[�^�[�Q�b�g�r���[
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_device->CreateRenderTargetView(bxTexture, &rtvd, &_BXrenderertargetview);

		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		_device->CreateShaderResourceView(bxTexture, &srvd, &_BXshaderresourceview);

		bxTexture->Release();
	}
	//�u���[Y�e�N�X�`���̍쐬
	{
		ID3D11Texture2D* byTexture = NULL;

		D3D11_TEXTURE2D_DESC	td;		//�e�N�X�`���쐬�p�f�X�N���v�^�\����
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;

		//�쐬����~�b�v�}�b�v�̐�
		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//�s�N�Z���t�H�[�}�b�g
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//�����_�����O�^�[�Q�b�g�p�̐ݒ�
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//�e�N�X�`���̍쐬
		_device->CreateTexture2D(&td, NULL, &byTexture);

		//�����_�[�^�[�Q�b�g�r���[
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_device->CreateRenderTargetView(byTexture, &rtvd, &_BYrenderertargetview);

		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		_device->CreateShaderResourceView(byTexture, &srvd, &_BYshaderresourceview);

		byTexture->Release();
	}
	//���[�V�����u���[��e�N�X�`���̍쐬
	{
		ID3D11Texture2D* mbTexture = NULL;

		D3D11_TEXTURE2D_DESC	td;		//�e�N�X�`���쐬�p�f�X�N���v�^�\����
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;

		//�쐬����~�b�v�}�b�v�̐�
		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//�s�N�Z���t�H�[�}�b�g
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//�����_�����O�^�[�Q�b�g�p�̐ݒ�
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//�e�N�X�`���̍쐬
		_device->CreateTexture2D(&td, NULL, &mbTexture);

		//�����_�[�^�[�Q�b�g�r���[
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_device->CreateRenderTargetView(mbTexture, &rtvd, &_MBrenderertargetview);

		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		_device->CreateShaderResourceView(mbTexture, &srvd, &_MBshaderresourceview);

		mbTexture->Release();
	}
	{
		//�[�x
		ID3D11Texture2D* depthTexture = NULL;
		D3D11_TEXTURE2D_DESC	dtd;			//�e�N�X�`���쐬�p�f�X�N���v�^�\����
		ZeroMemory(&dtd, sizeof(dtd));

		dtd.Width = swapChainDesc.BufferDesc.Width;
		dtd.Height = swapChainDesc.BufferDesc.Height;
		//�쐬����~�b�v�}�b�v�̐�
		dtd.MipLevels = 1;
		dtd.ArraySize = 1;
		dtd.Format = DXGI_FORMAT_R32_TYPELESS;		//�s�N�Z���t�H�[�}�b�g
		dtd.SampleDesc = swapChainDesc.SampleDesc;
		dtd.Usage = D3D11_USAGE_DEFAULT;
		//�����_�����O�^�[�Q�b�g�p�̐ݒ�
		dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		dtd.CPUAccessFlags = 0;
		dtd.MiscFlags = 0;

		//�e�N�X�`���̍쐬
		_device->CreateTexture2D(&dtd, NULL, &depthTexture);
		//�����_�[�^�[�Q�b�g�r���[
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		_device->CreateDepthStencilView(depthTexture, &dsvd, &_Depthstencilview);

		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R32_FLOAT;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		srvd.Texture2D.MostDetailedMip = 0;
		_device->CreateShaderResourceView(depthTexture, &srvd, &_Depthshaderresourceview);

		depthTexture->Release();
	}
	{
		for (int i = 0; i < 3; i++) {
			//���C�g����̐[�x
			ID3D11Texture2D* depthTexture = NULL;
			D3D11_TEXTURE2D_DESC	dtd;			//�e�N�X�`���쐬�p�f�X�N���v�^�\����
			ZeroMemory(&dtd, sizeof(dtd));

			dtd.Width = 1024;
			dtd.Height = 1024;
			//�쐬����~�b�v�}�b�v�̐�
			dtd.MipLevels = 1;
			dtd.ArraySize = 1;
			dtd.Format = DXGI_FORMAT_R32_TYPELESS;		//�s�N�Z���t�H�[�}�b�g
			dtd.SampleDesc = swapChainDesc.SampleDesc;
			dtd.Usage = D3D11_USAGE_DEFAULT;
			//�����_�����O�^�[�Q�b�g�p�̐ݒ�
			dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			dtd.CPUAccessFlags = 0;
			dtd.MiscFlags = 0;

			//�e�N�X�`���̍쐬
			_device->CreateTexture2D(&dtd, NULL, &depthTexture);
			//�����_�[�^�[�Q�b�g�r���[
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
			ZeroMemory(&dsvd, sizeof(dsvd));
			dsvd.Format = DXGI_FORMAT_D32_FLOAT;
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			_device->CreateDepthStencilView(depthTexture, &dsvd, &_CameraDepthstencilview[i]);

			//�V�F�[�_�[���\�[�X�r���[�̍쐬
			D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
			ZeroMemory(&srvd, sizeof(srvd));
			srvd.Format = DXGI_FORMAT_R32_FLOAT;
			srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvd.Texture2D.MipLevels = 1;
			srvd.Texture2D.MostDetailedMip = 0;
			_device->CreateShaderResourceView(depthTexture, &srvd, &_CameraDepthshaderresourceview[i]);

			depthTexture->Release();
		}
	}
	{
		//���x�}�b�v
		D3D11_TEXTURE2D_DESC	dtd;			//�e�N�X�`���쐬�p�f�X�N���v�^�\����
		ZeroMemory(&dtd, sizeof(dtd));
		dtd.Width = swapChainDesc.BufferDesc.Width;
		dtd.Height = swapChainDesc.BufferDesc.Height;
		//�쐬����~�b�v�}�b�v�̐�
		dtd.MipLevels = 1;
		dtd.ArraySize = 1;
		dtd.Format = DXGI_FORMAT_R8G8_UNORM;		//�s�N�Z���t�H�[�}�b�g
		dtd.SampleDesc = swapChainDesc.SampleDesc;
		dtd.Usage = D3D11_USAGE_DEFAULT;
		//�����_�����O�^�[�Q�b�g�p�̐ݒ�
		dtd.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		dtd.CPUAccessFlags = 0;
		dtd.MiscFlags = 0;

		//�e�N�X�`���̍쐬
		ID3D11Texture2D* velocityTexture = NULL;
		_device->CreateTexture2D(&dtd, NULL, &velocityTexture);

		//�����_�[�^�[�Q�b�g�r���[
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_device->CreateRenderTargetView(velocityTexture, &rtvd, &_Velrenderertargetview);

		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		srvd.Texture2D.MostDetailedMip = 0;
		_device->CreateShaderResourceView(velocityTexture, &srvd, &_Velshaderresourceview);

		velocityTexture->Release();
	}
	//�����_�����O�e�N�X�`���̍쐬
	{
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC	td;		//�e�N�X�`���쐬�p�f�X�N���v�^�\����
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;

		//�쐬����~�b�v�}�b�v�̐�
		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//�s�N�Z���t�H�[�}�b�g
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//�����_�����O�^�[�Q�b�g�p�̐ݒ�
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//�e�N�X�`���̍쐬
		_device->CreateTexture2D(&td, NULL, &ppTexture);

		//�����_�[�^�[�Q�b�g�r���[
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_device->CreateRenderTargetView(ppTexture, &rtvd, &_PErenderertargetview);

		//�V�F�[�_�[���\�[�X�r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		_device->CreateShaderResourceView(ppTexture, &srvd, &_PEshaderresourceview);

		ppTexture->Release();
	}
}



void Renderer::Uninit() {

	_worldbuffer->Release();
	_viewbuffer->Release();
	_projectionbuffer->Release();
	_prevworldbuffer->Release();
	_prevviewbuffer->Release();
	_prevprojectionbuffer->Release();
	_lightbuffer->Release();
	_materialbuffer->Release();
	_parameterbuffer->Release();
	_weightsbuffer->Release();
	_dofbuffer->Release();
	_cascadesplitbuffer->Release();

	_Velshaderresourceview->Release();
	_Velrenderertargetview->Release();
	_MBshaderresourceview->Release();
	_MBrenderertargetview->Release();
	for (int i = 0; i < 3; i++) {
		_CameraDepthshaderresourceview[i]->Release();
		_CameraDepthstencilview[i]->Release();
	}
	_Depthshaderresourceview->Release();
	_Depthstencilview->Release();
	_BXshaderresourceview->Release();
	_BXrenderertargetview->Release();
	_BYshaderresourceview->Release();
	_BYrenderertargetview->Release();
	_PEshaderresourceview->Release();
	_PErenderertargetview->Release();

	_RendertargetTEX->Release();

	_devicecontext->ClearState();
	_rendertargetview->Release();
	_swapchain->Release();
	_devicecontext->Release();
	_device->Release();

}




void Renderer::Begin() {
	_devicecontext->OMSetRenderTargets(1, &_rendertargetview, _depthstencilview);
	//�w�i�F
	float clearColor[4] = { 0.564f, 0.933f, 0.564f, 1.0f };
	_devicecontext->ClearRenderTargetView(_rendertargetview, clearColor);
	_devicecontext->ClearDepthStencilView(_depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End() {
	_swapchain->Present(1, 0);	//���������O�ɂ���Ɛ��������I�t
}




void Renderer::SetDepthEnable(bool Enable) {
	if (Enable)
		_devicecontext->OMSetDepthStencilState(_depthstateenable, NULL);
	else
		_devicecontext->OMSetDepthStencilState(_depthstatedisable, NULL);

}



void Renderer::SetATCEnable(bool Enable) {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		_devicecontext->OMSetBlendState(_blendstateATC, blendFactor, 0xffffffff);
	else
		_devicecontext->OMSetBlendState(_blendstate, blendFactor, 0xffffffff);

}

void Renderer::SetBlendAddEnable(bool Enable) {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		_devicecontext->OMSetBlendState(_blendstateAdd, blendFactor, 0xffffffff);
	else
		_devicecontext->OMSetBlendState(_blendstate, blendFactor, 0xffffffff);
}

void Renderer::SetWorldViewProjection2D() {
	SetWorldMatrix(XMMatrixIdentity(), _prevworld);
	SetViewMatrix(XMMatrixIdentity(), _prevview);

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection, _prevprojection);
}


void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix) {
	XMFLOAT4X4 worldf;

	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	_devicecontext->UpdateSubresource(_worldbuffer, 0, NULL, &worldf, 0, 0);
}

void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix, XMMATRIX& PrevWorld) {
	XMFLOAT4X4 worldf;

	XMStoreFloat4x4(&worldf, XMMatrixTranspose(PrevWorld));
	_devicecontext->UpdateSubresource(_prevworldbuffer, 0, NULL, &worldf, 0, 0);

	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	_devicecontext->UpdateSubresource(_worldbuffer, 0, NULL, &worldf, 0, 0);

	//�O�t���[���ۑ�
	PrevWorld = WorldMatrix;
}

void Renderer::SetViewMatrix(XMMATRIX ViewMatrix) {
	XMFLOAT4X4 viewf;

	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	_devicecontext->UpdateSubresource(_viewbuffer, 0, NULL, &viewf, 0, 0);
}
void Renderer::SetViewMatrix(XMMATRIX ViewMatrix, XMMATRIX& PrevView) {
	XMFLOAT4X4 viewf;

	XMStoreFloat4x4(&viewf, XMMatrixTranspose(PrevView));
	_devicecontext->UpdateSubresource(_prevviewbuffer, 0, NULL, &viewf, 0, 0);

	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	_devicecontext->UpdateSubresource(_viewbuffer, 0, NULL, &viewf, 0, 0);

	//�O�t���[���ۑ�
	PrevView = ViewMatrix;
}

void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix) {
	XMFLOAT4X4 projectionf;

	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	_devicecontext->UpdateSubresource(_projectionbuffer, 0, NULL, &projectionf, 0, 0);
}
void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix, XMMATRIX& PrevProjection) {
	XMFLOAT4X4 projectionf;

	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(PrevProjection));
	_devicecontext->UpdateSubresource(_prevprojectionbuffer, 0, NULL, &projectionf, 0, 0);

	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	_devicecontext->UpdateSubresource(_projectionbuffer, 0, NULL, &projectionf, 0, 0);

	//�O�t���[���ۑ�
	PrevProjection = ProjectionMatrix;
}



void Renderer::SetMaterial(MATERIAL Material) {
	_devicecontext->UpdateSubresource(_materialbuffer, 0, NULL, &Material, 0, 0);
}

void Renderer::SetLight(LIGHT Light) {
	_devicecontext->UpdateSubresource(_lightbuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCameraPosition(XMFLOAT3 Position) {
	XMFLOAT4 temp = XMFLOAT4(Position.x, Position.y, Position.z, 1.0f);
	_devicecontext->UpdateSubresource(_camerabuffer, 0, NULL, &temp, 0, 0);
}

void Renderer::SetParameter(XMFLOAT4 Parameter) {
	_devicecontext->UpdateSubresource(_parameterbuffer, 0, NULL, &Parameter, 0, 0);
}

void Renderer::SetWeights(float* weights) {
	_devicecontext->UpdateSubresource(_weightsbuffer, 0, NULL, &weights[0], 0, 0);
}

void Renderer::SetDoF(XMFLOAT2 dof) {
	_devicecontext->UpdateSubresource(_dofbuffer, 0, NULL, &dof, 0, 0);
}

void Renderer::SetCascadeSplit(float* split) {
	_devicecontext->UpdateSubresource(_cascadesplitbuffer, 0, NULL, &split, 0, 0);
}





void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName) {

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	_device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	_device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName) {
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	_device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}


void Renderer::SetViewportSize(const XMFLOAT2& size) {
	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT viewport;
	viewport.Width = size.x;
	viewport.Height = size.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	_devicecontext->RSSetViewports(1, &viewport);
}

void Renderer::BeginPE() {
	SetViewportSize({ (FLOAT)SCREEN_WIDTH, (FLOAT)SCREEN_HEIGHT });

	ID3D11RenderTargetView* mrt[2]{
		_PErenderertargetview, _Velrenderertargetview
	};
	_devicecontext->OMSetRenderTargets(2,
		mrt,	//�����_�����O�e�N�X�`�� 
		_Depthstencilview);		//Z�o�b�t�@

	//�����_�����O�e�N�X�`���N���A
	float ClearColor[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
	_devicecontext->ClearRenderTargetView(_PErenderertargetview, ClearColor);
	_devicecontext->ClearRenderTargetView(_Velrenderertargetview, ClearColor);

	//Z�o�b�t�@�N���A
	_devicecontext->ClearDepthStencilView(_Depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginBlurX() {
	_devicecontext->OMSetRenderTargets(1,
		&_BXrenderertargetview,	//�����_�����O�e�N�X�`�� 
		_depthstencilview);		//Z�o�b�t�@

	//�����_�����O�e�N�X�`���N���A
	float ClearColor[4] = { 0.0f, 0.5f, 0.0f, 1.0f };
	_devicecontext->ClearRenderTargetView(_BXrenderertargetview, ClearColor);

	//Z�o�b�t�@�N���A
	_devicecontext->ClearDepthStencilView(_depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginBlurY() {
	_devicecontext->OMSetRenderTargets(1,
		&_BYrenderertargetview,	//�����_�����O�e�N�X�`�� 
		_depthstencilview);		//Z�o�b�t�@

	//�����_�����O�e�N�X�`���N���A
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_devicecontext->ClearRenderTargetView(_BYrenderertargetview, ClearColor);

	//Z�o�b�t�@�N���A
	_devicecontext->ClearDepthStencilView(_depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginMotionBlur() {
	_devicecontext->OMSetRenderTargets(1,
		&_MBrenderertargetview,	//�����_�����O�e�N�X�`�� 
		_depthstencilview);		//Z�o�b�t�@

	//�����_�����O�e�N�X�`���N���A
	float ClearColor[4] = { 0.5f, 0.0f, 0.0f, 1.0f };
	_devicecontext->ClearRenderTargetView(_MBrenderertargetview, ClearColor);

	//Z�o�b�t�@�N���A
	_devicecontext->ClearDepthStencilView(_depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginLightDepth(const int& idx) {
	SetViewportSize({ 1024.0f, 1024.0f });

	_devicecontext->OMSetRenderTargets(0, NULL, _CameraDepthstencilview[idx]);
	_devicecontext->ClearDepthStencilView(_CameraDepthstencilview[idx], D3D11_CLEAR_DEPTH, 1.0f, 0);
}



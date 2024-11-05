#include "Main.h"
#include "Renderer.h"
#include <io.h>


D3D_FEATURE_LEVEL       Renderer::_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::_Device{};
ID3D11DeviceContext*    Renderer::_DeviceContext{};
IDXGISwapChain*         Renderer::_SwapChain{};
ID3D11RenderTargetView* Renderer::_RenderTargetView{};
ID3D11DepthStencilView* Renderer::_DepthStencilView{};

ID3D11Buffer*			Renderer::_WorldBuffer{};
ID3D11Buffer*			Renderer::_ViewBuffer{};
ID3D11Buffer*			Renderer::_ProjectionBuffer{};
ID3D11Buffer*			Renderer::_MaterialBuffer{};
ID3D11Buffer*			Renderer::_LightBuffer{};
ID3D11Buffer*			Renderer::_CameraBuffer{};
ID3D11Buffer*			Renderer::_ParameterBuffer{};


ID3D11DepthStencilState* Renderer::_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::_DepthStateDisable{};


ID3D11BlendState*		Renderer::_BlendState{};
ID3D11BlendState*		Renderer::_BlendStateAdd{};
ID3D11BlendState*		Renderer::_BlendStateATC{};


ID3D11RenderTargetView* Renderer::_PERendererTargetView = NULL;
ID3D11ShaderResourceView* Renderer::_PEShaderResourceView = NULL;

ID3D11RenderTargetView* Renderer::_BXRendererTargetView = NULL;
ID3D11ShaderResourceView* Renderer::_BXShaderResourceView = NULL;
ID3D11RenderTargetView* Renderer::_BYRendererTargetView = NULL;
ID3D11ShaderResourceView* Renderer::_BYShaderResourceView = NULL;



void Renderer::Init()
{
	HRESULT hr = S_OK;




	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										D3D10_CREATE_DEVICE_BGRA_SUPPORT,
										NULL,
										0,
										D3D11_SDK_VERSION,
										&swapChainDesc,
										&_SwapChain,
										&_Device,
										&_FeatureLevel,
										&_DeviceContext );






	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&renderTarget );
	_Device->CreateRenderTargetView( renderTarget, NULL, &_RenderTargetView );
	renderTarget->Release();


	// デプスステンシルバッファ作成
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
	_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &_DepthStencilView);
	depthStencile->Release();


	_DeviceContext->OMSetRenderTargets(1, &_RenderTargetView, _DepthStencilView);





	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	_DeviceContext->RSSetViewports( 1, &viewport );



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; 
	rasterizerDesc.CullMode = D3D11_CULL_BACK; 
	rasterizerDesc.DepthClipEnable = TRUE; 
	rasterizerDesc.MultisampleEnable = FALSE; 

	ID3D11RasterizerState *rs;
	_Device->CreateRasterizerState( &rasterizerDesc, &rs );

	_DeviceContext->RSSetState( rs );




	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//前景のαにかける式(* A)
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//背景のαにかける式(1 - A)
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_Device->CreateBlendState( &blendDesc, &_BlendState );


	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;				//背景のαにかける式(* 1)
	_Device->CreateBlendState( &blendDesc, &_BlendStateAdd );


	blendDesc.AlphaToCoverageEnable = TRUE;
	_Device->CreateBlendState( &blendDesc, &_BlendStateATC );

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	_DeviceContext->OMSetBlendState(_BlendState, blendFactor, 0xffffffff );





	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	_Device->CreateDepthStencilState( &depthStencilDesc, &_DepthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	_Device->CreateDepthStencilState( &depthStencilDesc, &_DepthStateDisable );//深度無効ステート

	_DeviceContext->OMSetDepthStencilState( _DepthStateEnable, NULL );




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	_Device->CreateSamplerState( &samplerDesc, &samplerState );

	_DeviceContext->PSSetSamplers( 0, 1, &samplerState );



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	_Device->CreateBuffer( &bufferDesc, NULL, &_WorldBuffer );
	_DeviceContext->VSSetConstantBuffers( 0, 1, &_WorldBuffer);

	_Device->CreateBuffer( &bufferDesc, NULL, &_ViewBuffer );
	_DeviceContext->VSSetConstantBuffers( 1, 1, &_ViewBuffer );

	_Device->CreateBuffer( &bufferDesc, NULL, &_ProjectionBuffer );
	_DeviceContext->VSSetConstantBuffers( 2, 1, &_ProjectionBuffer );


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	_Device->CreateBuffer( &bufferDesc, NULL, &_MaterialBuffer );
	_DeviceContext->VSSetConstantBuffers( 3, 1, &_MaterialBuffer );
	_DeviceContext->PSSetConstantBuffers( 3, 1, &_MaterialBuffer );


	bufferDesc.ByteWidth = sizeof(LIGHT);

	_Device->CreateBuffer( &bufferDesc, NULL, &_LightBuffer );
	_DeviceContext->VSSetConstantBuffers( 4, 1, &_LightBuffer );
	_DeviceContext->PSSetConstantBuffers( 4, 1, &_LightBuffer );


	bufferDesc.ByteWidth = sizeof(XMFLOAT4);

	_Device->CreateBuffer(&bufferDesc, NULL, &_CameraBuffer);
	_DeviceContext->PSSetConstantBuffers(5, 1, &_CameraBuffer);


	bufferDesc.ByteWidth = sizeof(XMFLOAT4);

	_Device->CreateBuffer(&bufferDesc, NULL, &_ParameterBuffer);
	_DeviceContext->PSSetConstantBuffers(6, 1, &_ParameterBuffer);



	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	light.Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = XMFLOAT4(1.5f, 1.5f, 1.5f, 1.0f);
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	SetMaterial(material);


	//09_13追加---
	//ブラーXテクスチャの作成
	{
		ID3D11Texture2D* bxTexture = NULL;

		D3D11_TEXTURE2D_DESC	td;		//テクスチャ作成用デスクリプタ構造体
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width * 0.5f;
		td.Height = swapChainDesc.BufferDesc.Height;

		//作成するミップマップの数
		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//ピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//レンダリングターゲット用の設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//テクスチャの作成
		_Device->CreateTexture2D(&td, NULL, &bxTexture);

		//レンダーターゲットビュー
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_Device->CreateRenderTargetView(bxTexture, &rtvd, &_BXRendererTargetView);

		//シェーダーリソースビューの作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		_Device->CreateShaderResourceView(bxTexture, &srvd, &_BXShaderResourceView);
	}
	//ブラーYテクスチャの作成
	{
		ID3D11Texture2D* byTexture = NULL;

		D3D11_TEXTURE2D_DESC	td;		//テクスチャ作成用デスクリプタ構造体
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height * 0.5f;

		//作成するミップマップの数
		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//ピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//レンダリングターゲット用の設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//テクスチャの作成
		_Device->CreateTexture2D(&td, NULL, &byTexture);

		//レンダーターゲットビュー
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_Device->CreateRenderTargetView(byTexture, &rtvd, &_BYRendererTargetView);

		//シェーダーリソースビューの作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		_Device->CreateShaderResourceView(byTexture, &srvd, &_BYShaderResourceView);
	}
	//レンダリングテクスチャの作成
	{
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC	td;		//テクスチャ作成用デスクリプタ構造体
		ZeroMemory(&td, sizeof(td));

		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;

		//作成するミップマップの数
		td.MipLevels = 1;
		td.ArraySize = 1;

		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//ピクセルフォーマット
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;

		//レンダリングターゲット用の設定
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		//テクスチャの作成
		_Device->CreateTexture2D(&td, NULL, &ppTexture);

		//レンダーターゲットビュー
		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		_Device->CreateRenderTargetView(ppTexture, &rtvd, &_PERendererTargetView);

		//シェーダーリソースビューの作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		_Device->CreateShaderResourceView(ppTexture, &srvd, &_PEShaderResourceView);
	}


}



void Renderer::Uninit()
{

	_WorldBuffer->Release();
	_ViewBuffer->Release();
	_ProjectionBuffer->Release();
	_LightBuffer->Release();
	_MaterialBuffer->Release();

	_BXShaderResourceView->Release();
	_BXRendererTargetView->Release();
	_BYShaderResourceView->Release();
	_BYRendererTargetView->Release();
	_PEShaderResourceView->Release();
	_PERendererTargetView->Release();


	_DeviceContext->ClearState();
	_RenderTargetView->Release();
	_SwapChain->Release();
	_DeviceContext->Release();
	_Device->Release();

}




void Renderer::Begin()
{
	_DeviceContext->OMSetRenderTargets(1, &_RenderTargetView, _DepthStencilView);
	//背景色
	float clearColor[4] = { 0.564f, 0.933f, 0.564f, 1.0f };
	_DeviceContext->ClearRenderTargetView( _RenderTargetView, clearColor );
	_DeviceContext->ClearDepthStencilView( _DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End()
{
	_SwapChain->Present( 1, 0 );
}




void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		_DeviceContext->OMSetDepthStencilState( _DepthStateEnable, NULL );
	else
		_DeviceContext->OMSetDepthStencilState( _DepthStateDisable, NULL );

}



void Renderer::SetATCEnable( bool Enable )
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		_DeviceContext->OMSetBlendState(_BlendStateATC, blendFactor, 0xffffffff);
	else
		_DeviceContext->OMSetBlendState(_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetBlendAddEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		_DeviceContext->OMSetBlendState(_BlendStateAdd, blendFactor, 0xffffffff);
	else
		_DeviceContext->OMSetBlendState(_BlendState, blendFactor, 0xffffffff);
}

void Renderer::SetWorldViewProjection2D()
{
	SetWorldMatrix(XMMatrixIdentity());
	SetViewMatrix(XMMatrixIdentity());

	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	SetProjectionMatrix(projection);
}


void Renderer::SetWorldMatrix(XMMATRIX WorldMatrix)
{
	XMFLOAT4X4 worldf;
	XMStoreFloat4x4(&worldf, XMMatrixTranspose(WorldMatrix));
	_DeviceContext->UpdateSubresource(_WorldBuffer, 0, NULL, &worldf, 0, 0);
}

void Renderer::SetViewMatrix(XMMATRIX ViewMatrix)
{
	XMFLOAT4X4 viewf;
	XMStoreFloat4x4(&viewf, XMMatrixTranspose(ViewMatrix));
	_DeviceContext->UpdateSubresource(_ViewBuffer, 0, NULL, &viewf, 0, 0);
}

void Renderer::SetProjectionMatrix(XMMATRIX ProjectionMatrix)
{
	XMFLOAT4X4 projectionf;
	XMStoreFloat4x4(&projectionf, XMMatrixTranspose(ProjectionMatrix));
	_DeviceContext->UpdateSubresource(_ProjectionBuffer, 0, NULL, &projectionf, 0, 0);

}



void Renderer::SetMaterial( MATERIAL Material )
{
	_DeviceContext->UpdateSubresource( _MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLight( LIGHT Light )
{
	_DeviceContext->UpdateSubresource(_LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCameraPosition(XMFLOAT3 Position)
{
	XMFLOAT4 temp = XMFLOAT4(Position.x, Position.y, Position.z, 1.0f);
	_DeviceContext->UpdateSubresource(_CameraBuffer, 0, NULL, &temp, 0, 0);
}

void Renderer::SetParameter(XMFLOAT4 Parameter)
{
	_DeviceContext->UpdateSubresource(_ParameterBuffer, 0, NULL, &Parameter, 0, 0);
}





void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}



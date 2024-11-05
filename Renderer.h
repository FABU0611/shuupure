#pragma once




struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
	XMFLOAT3	Tangent;
	float dummy;
};



struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	XMFLOAT4	Direction;
	XMFLOAT4	Diffuse;			//色
	XMFLOAT4	Ambient;
	XMFLOAT4	SkyColor;
	XMFLOAT4	GroundColor;
	XMFLOAT4	GroundNormal;
	XMFLOAT4	Position;			//光源の座標
	XMFLOAT4	PointLightParam;	//光の範囲
	XMFLOAT4	Angle;	//光の範囲
};



class Renderer
{
private:

	static D3D_FEATURE_LEVEL			_FeatureLevel;

	static ID3D11Device*				_Device;
	static ID3D11DeviceContext*			_DeviceContext;
	static IDXGISwapChain*				_SwapChain;
	static ID3D11RenderTargetView*		_RenderTargetView;
	static ID3D11DepthStencilView*		_DepthStencilView;

	static ID3D11Buffer*				_WorldBuffer;
	static ID3D11Buffer*				_ViewBuffer;
	static ID3D11Buffer*				_ProjectionBuffer;
	static ID3D11Buffer*				_MaterialBuffer;
	static ID3D11Buffer*				_LightBuffer;
	static ID3D11Buffer*				_CameraBuffer;
	static ID3D11Buffer*				_ParameterBuffer;


	static ID3D11DepthStencilState*		_DepthStateEnable;
	static ID3D11DepthStencilState*		_DepthStateDisable;

	static ID3D11BlendState*			_BlendState;
	static ID3D11BlendState*			_BlendStateAdd;
	static ID3D11BlendState*			_BlendStateATC;


	static ID3D11RenderTargetView*		_PERendererTargetView;
	static ID3D11ShaderResourceView*	_PEShaderResourceView;

	static ID3D11RenderTargetView*		_BXRendererTargetView;
	static ID3D11ShaderResourceView*	_BXShaderResourceView;
	static ID3D11RenderTargetView*		_BYRendererTargetView;
	static ID3D11ShaderResourceView*	_BYShaderResourceView;


public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static IDXGISwapChain* GetSwapChain() { return _SwapChain; }

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetBlendAddEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosition(XMFLOAT3 Position);
	static void SetParameter(XMFLOAT4 Parameter);

	static ID3D11Device* GetDevice( void ){ return _Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return _DeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


	static ID3D11ShaderResourceView* GetPETexture() { return _PEShaderResourceView; }
	static ID3D11ShaderResourceView* GetBXTexture() { return _BXShaderResourceView; }
	static ID3D11ShaderResourceView* GetBYTexture() { return _BYShaderResourceView; }

	//レンダリングターゲットをテクスチャに切り替える
	static void BeginPE() {
		_DeviceContext->OMSetRenderTargets(1,
			&_PERendererTargetView,	//レンダリングテクスチャ 
			_DepthStencilView);		//Zバッファ

		//レンダリングテクスチャクリア
		float ClearColor[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
		_DeviceContext->ClearRenderTargetView(_PERendererTargetView, ClearColor);

		//Zバッファクリア
		_DeviceContext->ClearDepthStencilView(_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
};

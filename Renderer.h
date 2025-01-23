//Renderer.h
//20106_田中　蓮
//24_04_24
#pragma once

//3D構造体
struct VERTEX_3D {
	XMFLOAT3	Position;
	XMFLOAT3	Normal;
	XMFLOAT4	Diffuse;
	XMFLOAT2	TexCoord;
	XMFLOAT3	Tangent;
	float		dummy;
};


//マテリアル構造体
struct MATERIAL {
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};


//ライト構造体
struct LIGHT {
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
	XMFLOAT4	Angle;				//光の角度
	XMMATRIX	ViewMatrix[4];
	XMMATRIX	ProjectionMatrix[4];
};



class Renderer {
public:
	static const int					CASCADE_NUM = 4;

private:
	static D3D_FEATURE_LEVEL			_featurelevel;

	static ID3D11Device*				_device;
	static ID3D11DeviceContext*			_devicecontext;
	static IDXGISwapChain*				_swapchain;
	static ID3D11RenderTargetView*		_rendertargetview;
	static ID3D11DepthStencilView*		_depthstencilview;

	static ID3D11Buffer*				_worldbuffer;
	static ID3D11Buffer*				_viewbuffer;
	static ID3D11Buffer*				_projectionbuffer;
	static ID3D11Buffer*				_prevworldbuffer;
	static ID3D11Buffer*				_prevviewbuffer;
	static ID3D11Buffer*				_prevprojectionbuffer;

	static ID3D11Buffer*				_materialbuffer;
	static ID3D11Buffer*				_lightbuffer;
	static ID3D11Buffer*				_camerabuffer;
	static ID3D11Buffer*				_parameterbuffer;
	static ID3D11Buffer*				_weightsbuffer;
	static ID3D11Buffer*				_dofbuffer;
	static ID3D11Buffer*				_cascadesplitbuffer;

	static XMMATRIX						_prevworld;
	static XMMATRIX						_prevview;
	static XMMATRIX						_prevprojection;

	static ID3D11DepthStencilState*		_depthstateenable;
	static ID3D11DepthStencilState*		_depthstatedisable;

	static ID3D11BlendState*			_blendstate;
	static ID3D11BlendState*			_blendstateAdd;
	static ID3D11BlendState*			_blendstateATC;


	static ID3D11RenderTargetView*		_sceneRTV;
	static ID3D11ShaderResourceView*	_sceneSRV;

	static ID3D11DepthStencilView*		_sceneDSV;
	static ID3D11ShaderResourceView*	_depthSRV;
	static ID3D11DepthStencilView*		_cameraDSV[CASCADE_NUM];
	static ID3D11ShaderResourceView*	_cameradepthSRV[CASCADE_NUM];
	static ID3D11RenderTargetView*		_bxRTV;
	static ID3D11ShaderResourceView*	_bxSRV;
	static ID3D11RenderTargetView*		_byRTV;
	static ID3D11ShaderResourceView*	_bySRV;
	static ID3D11RenderTargetView*		_velRTV;
	static ID3D11ShaderResourceView*	_velSRV;
	static ID3D11RenderTargetView*		_mbRTV;
	static ID3D11ShaderResourceView*	_mbSRV;

	static ID3D11Texture2D*				_rendertargetTEX;

public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static IDXGISwapChain* GetSwapChain() { return _swapchain; }

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetBlendAddEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetWorldMatrix(XMMATRIX WorldMatrix, XMMATRIX& PrevWorld);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix, XMMATRIX& PrevView);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix, XMMATRIX& PrevProjection);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosition(XMFLOAT3 Position);
	static void SetParameter(XMFLOAT4 Parameter);
	static void SetWeights(float* weights);
	static void SetDoF(XMFLOAT2 dof);
	static void SetCascadeSplit(float* split);

	static ID3D11Device* GetDevice(void) { return _device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return _devicecontext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


	static ID3D11ShaderResourceView* GetPETexture() { return _sceneSRV; }
	static ID3D11ShaderResourceView* GetBXTexture() { return _bxSRV; }
	static ID3D11ShaderResourceView* GetBYTexture() { return _bySRV; }
	static ID3D11ShaderResourceView* GetDepthTexture() { return _depthSRV; }
	static ID3D11ShaderResourceView* GetCameraDepthTexture(const int& idx) { return _cameradepthSRV[idx]; }
	static ID3D11ShaderResourceView* GetCameraDepthTexture() { return (*_cameradepthSRV); }
	static ID3D11ShaderResourceView* GetVelocityTexture() { return _velSRV; }
	static ID3D11ShaderResourceView* GetMBTexture() { return _mbSRV; }

	static ID3D11Texture2D* GetRendertargetTEX() { return _rendertargetTEX; }

	static void SetViewportSize(const XMFLOAT2& size);

	//レンダリングターゲットをテクスチャに切り替える
	static void BeginPE();
	static void BeginBlurX();
	static void BeginBlurY();
	static void BeginMotionBlur();
	/// <summary>
	/// 使用前にライトのViewMatrixとProjectionMatrixをセットしてください
	/// </summary>
	static void BeginLightDepth(const int& idx);
};

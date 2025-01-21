//CheckCameraDepth.h
//20106_�c���@�@
//25_01_17
#include "CheckCameraDepth.h"
#include "Renderer.h"
#include "ShaderManager.h"

void CheckCameraDepth::Init() {
	VERTEX_3D vertex[4 * 3];

	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(SCREEN_HEIGHT * 0.25f, 0.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(0.0f, SCREEN_HEIGHT * 0.25f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(SCREEN_HEIGHT * 0.25f, SCREEN_HEIGHT * 0.25f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	

	vertex[4].Position = XMFLOAT3(SCREEN_HEIGHT * 0.25f + 10.0f, 0.0f, 0.0f);
	vertex[4].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[4].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[4].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[5].Position = XMFLOAT3(SCREEN_HEIGHT * 0.5f + 10.0f, 0.0f, 0.0f);
	vertex[5].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[5].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[5].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[6].Position = XMFLOAT3(SCREEN_HEIGHT * 0.25f + 10.0f, SCREEN_HEIGHT * 0.25f, 0.0f);
	vertex[6].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[6].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[6].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[7].Position = XMFLOAT3(SCREEN_HEIGHT * 0.5f + 10.0f, SCREEN_HEIGHT * 0.25f, 0.0f);
	vertex[7].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[7].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[7].TexCoord = XMFLOAT2(1.0f, 1.0f);

	
	vertex[8].Position = XMFLOAT3(SCREEN_HEIGHT * 0.5f + 20.0f, 0.0f, 0.0f);
	vertex[8].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[8].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[8].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[9].Position = XMFLOAT3(SCREEN_HEIGHT * 0.75f + 20.0f, 0.0f, 0.0f);
	vertex[9].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[9].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[9].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[10].Position = XMFLOAT3(SCREEN_HEIGHT * 0.5f + 20.0f, SCREEN_HEIGHT * 0.25f, 0.0f);
	vertex[10].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[10].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[11].Position = XMFLOAT3(SCREEN_HEIGHT * 0.75f + 20.0f, SCREEN_HEIGHT * 0.25f, 0.0f);
	vertex[11].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[11].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * (4 * 3);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_vertexbuffer);
}

void CheckCameraDepth::Uninit() {
	_vertexbuffer->Release();
}

void CheckCameraDepth::Update() {}

void CheckCameraDepth::Draw() {
	Shader::SetShader(ShaderName::Unlit);


	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for (int i = 0; i < 3; i++) {
		// �e�N�X�`���ݒ�
		ID3D11ShaderResourceView* ppTexture = Renderer::GetCameraDepthTexture(i);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &ppTexture);

		// �|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 4 * i);
	}
}

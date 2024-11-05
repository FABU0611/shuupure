//ParticleEmitter.cpp
//20106_�c���@�@
//2024_07_05
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "ParticleEmitter.h"
#include "Scene.h"
#include <random>
#include "ParticleFireworks.h"

//����������
void ParticleEmitter::Init() {
	VERTEX_3D vertex[4];

	for (int i = 0; i < 4; i++) {
		vertex[i].Normal = { 0.0f, 0.0f, 0.0f };
		vertex[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	vertex[0].Position = { -_size, _size, 0.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { _size, _size, 0.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { -_size, -_size, 0.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { _size, -_size, 0.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//���_�o�b�t�@����
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


	//�e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(_texname, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_texture);
	assert(_texture);


	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");
}

//�I������
void ParticleEmitter::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexbuffer->Release();
	_texture->Release();

	_VertexShader->Release();
	_PixelShader->Release();
	_VertexLayout->Release();
}

//�X�V����
void ParticleEmitter::Update() {}

//�`�揈��
void ParticleEmitter::Draw() {
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	//�J�����̃r���[�}�g���N�X�擾
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameobject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();

	//�r���[�̋t�s��
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);	//�t�s��
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);


	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Z�o�b�t�@����
	Renderer::SetDepthEnable(false);	//�p�[�e�B�N����Z�\�[�g����͕̂��ׂ�����
	//���Z�����ɕύX
	Renderer::SetBlendAddEnable(true);

	//�p�[�e�B�N���\��
	for (int i = 0; i < PARTICLE_MAX; i++) {
		if (!_particle[i].Enable) {
			continue;
		}
		//�}�e���A���ݒ�
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = _particle[i].Color;
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		//���[���h�}�g���N�X�ݒ�
		XMMATRIX world, scale, trans;
		scale = XMMatrixScaling(_particle[i].Size.x, _particle[i].Size.y, _particle[i].Size.z);
		trans = XMMatrixTranslation(_particle[i].Pos.x, _particle[i].Pos.y, _particle[i].Pos.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(world);

		//�|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
	//���Z�����𖳌�
	Renderer::SetBlendAddEnable(false);
	//Z�o�b�t�@�L��
	Renderer::SetDepthEnable(true);

	for (auto c : _components) {
		c->Draw();
	}
}

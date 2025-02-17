//C_Srite3D.cpp	3D�e�N�X�`���\��
//20106_�c���@�@
//24_07_17
#include "Main.h"
#include "Renderer.h"
#include "Scene.h"
#include "Manager.h"
#include "ParticleEmitter.h"
#include "C_Sprite3D.h"

void Sprite3D::Draw() {
	_scl = _gameobject->GetScale();
	_pos = _gameobject->GetPosition();

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(_vertexbuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//���_�ݒ�---------------------------------------------------------------------------
	vertex[0].Position = { -_scl.x,  _scl.y, 0.0f };
	vertex[1].Position = { _scl.x,  _scl.y, 0.0f };
	vertex[2].Position = { -_scl.x, -_scl.y, 0.0f };
	vertex[3].Position = { _scl.x, -_scl.y, 0.0f };

	for (int i = 0; i < 4; i++) {
		vertex[i].Tangent = _tangent;
	}

	if (_mode == DrawMode3D::Normal) {
		_rot = _gameobject->GetRotation();

		// �����I�ȉ�]�s����v�Z
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(_rot.x), XMConvertToRadians(_rot.y), XMConvertToRadians(_rot.z));

		XMVECTOR normal = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

		for (int i = 0; i < 4; i++) {
			vertex[i].Normal = _normal;

			// ���̒��_���W�𒆐S�Ɉړ�
			XMVECTOR originalPos = XMVectorSet(vertex[i].Position.x, vertex[i].Position.y, vertex[i].Position.z, 1.0f);

			// �V�����ʒu���v�Z
			XMVECTOR rotatedPosition = XMVector3TransformCoord(originalPos, rotationMatrix);
			rotatedPosition += XMLoadFloat3(&_pos);

			// �v�Z���ʂ�ݒ�
			XMStoreFloat3(&vertex[i].Position, rotatedPosition);


			XMVECTOR rotatedNormal = XMVector3TransformNormal(normal, rotationMatrix);
			XMStoreFloat3(&vertex[i].Normal, rotatedNormal);
		}

		//���[���h�}�g���N�X�ݒ�
		XMMATRIX world, scale, rot, trans;
		scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		trans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(world, _prevworld);
	}
	else if (_mode == DrawMode3D::Billboard ||
		_mode == DrawMode3D::BillboardColor) {
		//�J�����̃r���[�}�g���N�X�擾
		Scene* scene = Manager::GetScene();
		Camera* camera = scene->GetGameobject<Camera>();
		XMMATRIX view = camera->GetViewMatrix();

		//�r���[�̋t�s��
		XMMATRIX invView = XMMatrixInverse(nullptr, view);
		invView.r[3].m128_f32[0] = 0.0f;
		invView.r[3].m128_f32[1] = 0.0f;
		invView.r[3].m128_f32[2] = 0.0f;

		//���[���h�}�g���N�X�ݒ�
		XMMATRIX world, scale, trans;
		scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		trans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(world, _prevworld);
	}

	//�F�ݒ�-----------------------------------------------------------------------------
	if (_mode == DrawMode3D::Color ||
		_mode == DrawMode3D::BillboardColor) {
		vertex[0].Diffuse = _color;
		vertex[1].Diffuse = _color;
		vertex[2].Diffuse = _color;
		vertex[3].Diffuse = _color;
	}
	else {
		vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	//UV�ݒ�-----------------------------------------------------------------------------
	vertex[0].TexCoord = { _uv.x, _uv.y };
	vertex[1].TexCoord = { _uv.x + _uvend.x, _uv.y };
	vertex[2].TexCoord = { _uv.x, _uv.y + _uvend.y };
	vertex[3].TexCoord = { _uv.x + _uvend.x, _uv.y + _uvend.y };


	Renderer::GetDeviceContext()->Unmap(_vertexbuffer, 0);


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);
	if (_normaltexture != NULL) {
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &_normaltexture);
	}
	if (_envtexture != NULL) {
		Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &_envtexture);
	}

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (_mode == DrawMode3D::Particle) {
		//�J�����̃r���[�}�g���N�X�擾
		Scene* scene = Manager::GetScene();
		Camera* camera = scene->GetGameobject<Camera>();
		XMMATRIX view = camera->GetViewMatrix();

		//�r���[�̋t�s��
		XMMATRIX invView = XMMatrixInverse(nullptr, view);
		invView.r[3].m128_f32[0] = 0.0f;
		invView.r[3].m128_f32[1] = 0.0f;
		invView.r[3].m128_f32[2] = 0.0f;

		//���[���h�}�g���N�X�ݒ�
		XMMATRIX world, scale, trans;
		scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		trans = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(world, _prevworld);

		//Z�o�b�t�@����
		Renderer::SetDepthEnable(false);	//�p�[�e�B�N����Z�\�[�g����͕̂��ׂ�����
		//���Z�����ɕύX
		Renderer::SetBlendAddEnable(true);

		//�p�[�e�B�N���\��
		ParticleEmitter* emitter = dynamic_cast<ParticleEmitter*>(_gameobject);
		if (!emitter) {
			_mode = DrawMode3D::Normal;
			return;
		}
		Renderer::GetDeviceContext()->DrawInstanced(4, emitter->_instancecount, 0, 0);
		
		//���Z�����𖳌�
		Renderer::SetBlendAddEnable(false);
		//Z�o�b�t�@�L��
		Renderer::SetDepthEnable(true);
		return;
	}
	else {
		//�}�e���A���ݒ�
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);
	}
	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

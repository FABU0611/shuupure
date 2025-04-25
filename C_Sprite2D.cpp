//C_Sprite2D.cpp
//20106_�c���@�@
//24_07_16
#include "Main.h"
#include "Renderer.h"
#include "C_Sprite2D.h"


void Sprite2D::Draw(){
	//�}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(_vertexbuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//���_�ݒ�---------------------------------------------------------------------------
	if (_mode == DrawMode::LeftTop ||
		_mode == DrawMode::LeftTopColor) {
		vertex[0].Position = { _pos.x, _pos.y, 0.0f };
		vertex[1].Position = { _pos.x + _size.x, _pos.y, 0.0f };
		vertex[2].Position = { _pos.x, _pos.y + _size.y, 0.0f };
		vertex[3].Position = { _pos.x + _size.x, _pos.y + _size.y, 0.0f };
	}
	else if (_mode == DrawMode::Rotate ||
		_mode == DrawMode::RotateColor) {
		float hw, hh;
		hw = _size.x * 0.5f;
		hh = _size.y * 0.5f;

		//���_�ʒu�ݒ�
		vertex[0].Position = { _pos.x - hw, _pos.y - hh, 0.0f };
		vertex[1].Position = { _pos.x + hw, _pos.y - hh, 0.0f };
		vertex[2].Position = { _pos.x - hw, _pos.y + hh, 0.0f };
		vertex[3].Position = { _pos.x + hw, _pos.y + hh, 0.0f };

		//Z������̉�]�s����쐬
		XMMATRIX rotationMatrixZ = XMMatrixRotationZ(_rot.z);

		// �����I�ȉ�]�s����v�Z
		XMMATRIX rotationMatrix = rotationMatrixZ;

		for (int i = 0; i < 4; i++) {
			// ���̒��_���W�𒆐S�Ɉړ�
			XMVECTOR originalPos = XMVectorSet(vertex[i].Position.x - _pos.x, vertex[i].Position.y - _pos.y, vertex[i].Position.z - 0.0f, 1.0f);

			// ��]�s���K�p���ĐV�������W���v�Z
			XMVECTOR rotatedPos = XMVector3Transform(originalPos, rotationMatrix);

			// ���S���W���ēx���Z���āA�V�������W��ݒ�
			rotatedPos = XMVectorAdd(rotatedPos, XMVectorSet(_pos.x, _pos.y, 0.0f, 0.0f));
			XMStoreFloat3(&vertex[i].Position, rotatedPos);
			vertex[i].Normal = { 0.0f, 0.0f, 0.0f };
			vertex[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}
	else if (_mode == DrawMode::Multiply) {
		for (int i = 0; i < _dispNum; i++) {
			vertex[i * 4 + 0].Position = { _pos.x + _size.x * (0.25f * i) + (10.0f * i), _pos.y, 0.0f };
			vertex[i * 4 + 1].Position = { _pos.x + _size.x * (0.25f * (i + 1)) + (10.0f * i), _pos.y, 0.0f };
			vertex[i * 4 + 2].Position = { _pos.x + _size.x * (0.25f * i) + (10.0f * i), _pos.y + _size.y * 0.25f, 0.0f };
			vertex[i * 4 + 3].Position = { _pos.x + _size.x * (0.25f * (i + 1)) + (10.0f * i), _pos.y + _size.y * 0.25f, 0.0f };
		}
	}
	else {
		float hw, hh;
		hw = _size.x * 0.5f;
		hh = _size.y * 0.5f;

		vertex[0].Position = { _pos.x - hw, _pos.y - hh, 0.0f };
		vertex[1].Position = { _pos.x + hw, _pos.y - hh, 0.0f };
		vertex[2].Position = { _pos.x - hw, _pos.y + hh, 0.0f };
		vertex[3].Position = { _pos.x + hw, _pos.y + hh, 0.0f };
	}

	//�@���ݒ�---------------------------------------------------------------------------
	for (int i = 0; i < 4 * _dispNum; i++) {
		vertex[i].Normal = { 0.0f, 0.0f, -1.0f };
	}

	//�F�ݒ�-----------------------------------------------------------------------------
	if (_mode == DrawMode::Color ||
		_mode == DrawMode::LeftTopColor ||
		_mode == DrawMode::RotateColor) {
		vertex[0].Diffuse = _color;
		vertex[1].Diffuse = _color;
		vertex[2].Diffuse = _color;
		vertex[3].Diffuse = _color;
	}
	else {
		for (int i = 0; i < 4 * _dispNum; i++) {
			vertex[i].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}

	//UV�ݒ�-----------------------------------------------------------------------------
	if (_mode == DrawMode::Multiply) {
		for (int i = 0; i < _dispNum; i++) {
			vertex[i * 4 + 0].TexCoord = { 0.0f, 0.0f };
			vertex[i * 4 + 1].TexCoord = { 1.0f, 0.0f };
			vertex[i * 4 + 2].TexCoord = { 0.0f, 1.0f };
			vertex[i * 4 + 3].TexCoord = { 1.0f, 1.0f };
		}
	}
	else{
		vertex[0].TexCoord = { _uv.x, _uv.y };
		vertex[1].TexCoord = { _uv.x + _uvend.x, _uv.y };
		vertex[2].TexCoord = { _uv.x, _uv.y + _uvend.y };
		vertex[3].TexCoord = { _uv.x + _uvend.x, _uv.y + _uvend.y };
	}

	Renderer::GetDeviceContext()->Unmap(_vertexbuffer, 0);


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexbuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (_mode == DrawMode::Multiply) {
		//�e�N�X�`���ݒ�
		int i = 0;
		for (const auto& srv : _srv) {
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
			Renderer::GetDeviceContext()->Draw(4, 4 * i);
			i++;
		}
	}
	else {
		//�e�N�X�`���ݒ�
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_texture);
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
	_srv.clear();
}

void Sprite2D::Uninit() {
	Sprite::Uninit();

	_srv.clear();
}

//Rendpoyl.h
//20106_�c���@�@
//24_09_26

#pragma once


class Rendpoly{
	ID3D11Buffer*				_VertexBuffer = NULL;

	//������	�V�F�[�_�[�֘A�̕ϐ���ǉ�
	ID3D11VertexShader*			_VertexShader;	//���_�V�F�[�_�[�I�u�W�F�N�g�|�C���^
	ID3D11PixelShader*			_PixelShader;	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�|�C���^
	ID3D11InputLayout*			_VertexLayout;	//���_���C�A�E�g�I�u�W�F�N�g

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
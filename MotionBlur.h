//MotionBlur.h
//20106_�c���@�@
//24_11_08
#pragma once
#include "Main.h"

class MotionBlur {
	ID3D11Buffer* _vertexbuffer = NULL;

	ID3D11VertexShader* _vertexshader{};	//���_�V�F�[�_�[�I�u�W�F�N�g�|�C���^
	ID3D11PixelShader*	_pixelshader{};	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�|�C���^
	ID3D11InputLayout*	_vertexlayout{};	//���_���C�A�E�g�I�u�W�F�N�g

public:
	MotionBlur() {}
	~MotionBlur() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
};
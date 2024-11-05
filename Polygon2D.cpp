#include "Main.h"
#include "Renderer.h"
#include "Polygon2D.h"
#include "C_Sprite2D.h"

Polygon2D::Polygon2D(const XMFLOAT3& pos, const XMFLOAT3& size, const wchar_t* name){
	SetPosition(pos);
	_texname = name;

	//���S�ʒu�ƃT�C�Y���璸�_�ʒu���v�Z����
	_texpos[0].x = pos.x - size.x * 0.5f;
	_texpos[0].y = pos.y - size.y * 0.5f;

	_texpos[1].x = pos.x + size.x * 0.5f;
	_texpos[1].y = pos.y - size.y * 0.5f;

	_texpos[2].x = pos.x - size.x * 0.5f;
	_texpos[2].y = pos.y + size.y * 0.5f;

	_texpos[3].x = pos.x + size.x * 0.5f;
	_texpos[3].y = pos.y + size.y * 0.5f;

	_components.push_back(new Sprite2D(this));
	GetComponent<Sprite2D>()->SetSize(size);
}

//����������
void Polygon2D::Init(){
	GetComponent<Sprite2D>()->LoadTexture(_texname);
	GetComponent<Sprite2D>()->SetDrawMode(DrawMode::Normal);
	GetComponent<Sprite2D>()->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");
}

//�I������
void Polygon2D::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}

	_VertexShader->Release();
	_PixelShader->Release();
	_VertexLayout->Release();
}

//�X�V����
void Polygon2D::Update(){
	_rot += 0.1f;
	GetComponent<Sprite2D>()->SetRot({ 0.0f, 0.0f, _rot });
}

//�`�揈��
void Polygon2D::Draw(){
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	GetComponent<Sprite2D>()->SetPos(GetPosition());
	for (auto c : _components) {
		c->Draw();
	}
}

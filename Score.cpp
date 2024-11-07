#include "Main.h"
#include "Renderer.h"
#include "Score.h"
#include "C_Sprite2D.h"
#include "DataManager.h"

Score::Score(const XMFLOAT3& pos, const XMFLOAT3& size){
	SetPosition(pos);
	_texname = L"asset\\texture\\number.png";

	_ptn_width = (1.0f / _anime_ptn_x);
	_ptn_height = (1.0f / _anime_ptn_y);

	_score = 0;

	_components.push_back(new Sprite2D(this));
	_components.push_back(new Sprite2D(this));
	_components.push_back(new Sprite2D(this));
	for (auto c : _components) {
		Sprite2D* s = dynamic_cast<Sprite2D*>(c);
		if (!s) {
			continue;
		}
		s->SetSize(size);
	}
}

//����������
void Score::Init(){
	int count = 0;
	for (auto c : _components) {
		Sprite2D* s = dynamic_cast<Sprite2D*>(c);
		if (!s) {
			continue;
		}
		s->LoadTexture(_texname);
		s->SetDrawMode(DrawMode::Normal);
		s->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		s->SetPos({ GetPosition().x - s->GetSize().x * count, GetPosition().y, GetPosition().z });
		count++;
	}

	//�Z�[�u�f�[�^�����[�h����
	DataManager::LoadData(_rot, _twist, _dot);
	//�X�R�A�̌v�Z
	_score += _dot * 10;
	_score += _rot * 10;
	_score += _twist * 5;

	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\UnlitTextureVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\UnlitTexturePS.cso");
}

//�I������
void Score::Uninit(){
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

//�X�V����
void Score::Update(){
}

//�`�揈��
void Score::Draw(){
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

	int score = _score;
	//�\��
	for (auto c : _components) {
		Sprite2D* s = dynamic_cast<Sprite2D*>(c);
		if (!s) {
			continue;
		}
		_uv.x = _ptn_width * (score % 10);
		_uv.y = _ptn_height * (score % 10 / _anime_ptn_y);

		s->SetUV(_uv);
		s->SetUVend({ _ptn_width, _ptn_height });

		score = score / 10;

		s->Draw();
	}
}

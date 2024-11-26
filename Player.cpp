//Player.cpp
//20106_�c���@�@
//24_05_22
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Input.h"
#include "Player.h"
#include "C_AnimationModel.h"
#include "C_Transform.h"
#include "C_Collision.h"
#include "C_Audio.h"
#include "S_Game.h"

const float Player::FALL_NUM = -98.0f;
const float Player::JUMP_POWER = 100.0f;
const float Player::MAX_SPEED = 10.0f;
const float Player::MOVE_NUM = 10.0f;
const float Player::FRICTION_NUM = 2.0f;

Player::Player(const XMFLOAT3& pos, const XMFLOAT3& scl):
_state(nullptr){
	SetPosition(pos);
	SetScale(scl);
}

void Player::Init(){
	GetPosition().y = 0.0f;

	//Player�������Ă��邱�Ƃ�������
	AddComponent<AnimationModel>(this);
	AddComponent<Collision>(this);
	AddComponent<Audio>(this);

	//�R���W�����ݒ�
	GetComponent<Collision>()->SetPosition(GetPosition());
	GetComponent<Collision>()->SetRotation(GetRotation());
	GetComponent<Collision>()->SetScale({ 3.0f, 7.0f, 2.0f });
	GetComponent<Collision>()->SetType(ColiderType::BOX);

	//���f���ǂݍ���
	GetComponent<AnimationModel>()->Load("asset\\model\\bikeman.fbx");

	GetComponent<Audio>()->Load("asset\\audio\\jump.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\jump.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\splash01.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\splash01.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\splash02.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\splash02.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\splash03.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\splash03.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\bgm003.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\bgm003.wav", 0.1f);
	GetComponent<Audio>()->Play("asset\\audio\\bgm003.wav");

	//�V�F�[�_�[�Z�b�g
	Renderer::CreateVertexShader(&_vertexshader, &_vertexlayout,
		"shader\\NormalLightingVS.cso");

	Renderer::CreatePixelShader(&_pixelshader,
		"shader\\NormalLightingPS.cso");

	_accel.x = 25.0f;
	_accel.y = FALL_NUM;
	_accel.z = 25.0f;
	_move = { 0.0f, 0.0f, 0.0f };
	SetRotation({ 0.0f, 0.0f, 0.0f });

	//Y�������90�x��]������X��������������
	XMVECTOR yAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, XM_PIDIV2);
	XMStoreFloat4(&GetQuaternion(), rotationQuat);
}

void Player::Uninit() {
	for (auto c : _components) {
		c->Uninit();
	}

	_vertexshader->Release();
	_pixelshader->Release();
	_vertexlayout->Release();
}

void Player::Update() {
	if (Input::GetKeyTrigger(VK_SPACE)) {
		Manager::SetSceneFade<Game>(0.05f);
	}
	Scene* scene;
	scene = Manager::GetScene();

	XMFLOAT3 oldPos = GetPosition();

	_move = { 0.0f, 0.0f, 0.0f };
	float dt = Time::GetDeltaTime();
	for (auto c : _components) {
		c->Update();
	}

	SetQuaternion(GetQuaternion());

	//���x�̍X�V
	_velocity.x += _accel.x * _move.x * dt;
	_velocity.z += _accel.z * _move.z * dt;


	//���C
	_velocity.x -= _velocity.x * FRICTION_NUM * dt;
	_velocity.z -= _velocity.z * FRICTION_NUM * dt;


	//�ʒu�X�V
	GetPosition() += _velocity * dt;
}

void Player::Draw(){
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(_vertexlayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_vertexshader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_pixelshader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(GetScale().x, GetScale().y, GetScale().z);
	//�N�H�[�^�j�I���ŉ�]
	rot = XMMatrixRotationQuaternion(XMLoadFloat4(&GetQuaternion()));
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world, _prevworld);

	for (auto c : _components) {
		c->Draw();
	}
}

void Player::Hit(GameObject* obj){
}

void Player::PlayAudioBig() {
	PlayAudioMid();
	GetComponent<Audio>()->Play("asset\\audio\\splash03.wav", false);
}

void Player::PlayAudioMid() {
	PlayAudioSml();
	GetComponent<Audio>()->Play("asset\\audio\\splash02.wav", false);
}

void Player::PlayAudioSml() {
	GetComponent<Audio>()->Play("asset\\audio\\splash01.wav", false);
}

void Player::PlayAudioJump(){
	GetComponent<Audio>()->Play("asset\\audio\\jump.wav", false);
}

//camera.cpp
//20106_�c���@�@
//24_05_08
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "Scene.h"
#include "S_Title.h"
#include "Time.h"
#include "CameraState_Title1.h"
#include "CameraState_Game1.h"
#include "Manager.h"
#include "Gaussian.h"
#include "PostEffectManager.h"

const float Camera::NEAR_CLIP = 0.1f;
const float Camera::FAR_CLIP = 600.0f;

//�J����������𕪊�
void Camera::CalculationCascade() {
	int cascadenum = Renderer::CASCADE_NUM;

	float range = FAR_CLIP - NEAR_CLIP;
	float ratio = FAR_CLIP / NEAR_CLIP;

	for (int i = 0; i < cascadenum; i++) {
		float p = (i + 1) / (float)cascadenum;					//i = 0 -> 1/3, i = 1 -> 2/3...
		float logsplit = NEAR_CLIP * powf(ratio, p);			//�ΐ������@�߂��قǏ���������
		float unifromsplit = NEAR_CLIP + range * p;				//���`�����@�ϓ��ɕ���
		_cascade[i] = std::lerp(logsplit, unifromsplit, 0.5f);	//�u�����h���Ă���������	�������L������Ɖ𑜓x���Ⴍ�Ȃ�
	}
}

void Camera::Init() {
	CalculationCascade();

	SetPosition({ 0.0f, 25.0f, 30.0f });
	_target = { 35.0f, 6.3f, 15.0f };

	GetRotation() = { -0.76f, -0.8f, 0.6f };

	//���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = GetPosition().x - _target.x;
	vz = GetPosition().z - _target.z;
	_length = sqrtf(vx * vx + vz * vz);

	_cameraspeed = 0.07f;

	_fov = 1.0f;
	_aspect = (float)SCREEN_WIDTH / SCREEN_HEIGHT;


	Renderer::SetScreenParam({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, NEAR_CLIP, FAR_CLIP});


	Scene* scene;
	scene = Manager::GetScene();

	Title* title = dynamic_cast<Title*>(scene);
	if (title) {
		_state = new TitleState1(this);
		PostEffectManager::GetInstance()->GetGaussian()->SetDof({ 0.07f, 0.05f });
		PostEffectManager::GetInstance()->GetGaussian()->SetBoke(10.0f);
		return;
	}
	else {
		PostEffectManager::GetInstance()->GetGaussian()->SetDof({ 0.0f, 0.0f });
		PostEffectManager::GetInstance()->GetGaussian()->SetBoke(0.1f);
		_state = new GameState1(this);
	}
}

void Camera::Uninit() {
	delete _state;
}

void Camera::Update() {
	if (_state) {
		_state->Update();
	}

	XMFLOAT3 rot = GetRotation();

	//��]�s��̌v�Z
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);

	//��{�̃J�����ʒu
	XMVECTOR defaultPosition = XMVectorSet(0.0f, 0.0f, -_length, 0.0f);

	//�V�����J�����ʒu���v�Z
	XMVECTOR rotatedPosition = XMVector3TransformCoord(defaultPosition, rotationMatrix);
	rotatedPosition += XMLoadFloat3(&_target);

	//�v�Z���ʂ� _transform.position �ɐݒ�
	XMStoreFloat3(&GetPosition(), rotatedPosition);

	//���E�̊O��m���Ȃ��悤�ɗ��߂�
	float d = powf(GetPosition().x, 2) + powf(GetPosition().y, 2) + powf(GetPosition().z, 2);
	float r = powf(Manager::GetWorldRad(), 2);

	if (d > r) {
		XMFLOAT3 dir = VectorNormalize(GetPosition());
		SetPosition(dir * Manager::GetWorldRad());
	}

	//���ʂ�艺�ɍs���Ȃ��悤��
	if (GetPosition().y < 10.0f) {
		GetPosition().y = 10.0f;
	}
	if (_target.y < 0.0f) {
		_target.y = 0.0f;
	}

	GetComponent<Transform>()->Update();
}

void Camera::Draw() {
	//�r���[�}�g���N�X�ݒ�
	XMFLOAT3 up{ 0.0f, 1.0f, 0.0f };
	_viewmatrix = XMMatrixLookAtLH(XMLoadFloat3(&GetPosition()), XMLoadFloat3(&_target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(_viewmatrix, _prevview);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	_projectionMatrix = XMMatrixPerspectiveFovLH(_fov, _aspect, NEAR_CLIP, FAR_CLIP);

	Renderer::SetProjectionMatrix(_projectionMatrix, _prevprojection);

	Renderer::SetCameraPosition(GetPosition());
}

bool Camera::LerpFoV(const float& fov, const float& time) {
	if (fov == _fov) {
		return true;
	}
	if (!_islerping) {
		_startfov = _fov;
		_islerping = true;
	}

	_time += Time::GetDeltaTime();
	float d = std::clamp(_time / time, 0.0f, 1.0f);

	SetCameraFoV(std::lerp(_startfov, fov, d));

	if (d >= 1.0f) {
		_fov = fov;
		_time = 0.0f;
		_islerping = false;
		return true;
	}
	return false;
}

void Camera::ChangeState(CameraState* state) {
	if (!_state) {
		return;
	}
	delete _state;
	_state = state;
}

//������̂W���_�����[���h���W�ɂ��ďo��
std::vector<XMVECTOR> Camera::GetCornersWorldSpace(const float& nearZ, const float& farZ) const {
	std::vector<XMVECTOR> corners(8);

	//�ߖʂ̍����ƕ��̔���
	float nearHeight = tanf(_fov * 0.5f) * nearZ;
	float nearWidth = nearHeight * _aspect;
	//���ʂ̍����ƕ��̔���
	float farHeight = tanf(_fov * 0.5f) * farZ;
	float farWidth = farHeight * _aspect;

	//������̎l���̍��W���v�Z
	corners[0] = XMVectorSet(-nearWidth,  nearHeight, nearZ, 1.0f);
	corners[1] = XMVectorSet( nearWidth,  nearHeight, nearZ, 1.0f);
	corners[2] = XMVectorSet( nearWidth, -nearHeight, nearZ, 1.0f);
	corners[3] = XMVectorSet(-nearWidth, -nearHeight, nearZ, 1.0f);

	corners[4] = XMVectorSet(-farWidth,  farHeight, farZ, 1.0f);
	corners[5] = XMVectorSet( farWidth,  farHeight, farZ, 1.0f);
	corners[6] = XMVectorSet( farWidth, -farHeight, farZ, 1.0f);
	corners[7] = XMVectorSet(-farWidth, -farHeight, farZ, 1.0f);

	//�t�ϊ����ă��[���h��Ԃɒ���
	XMMATRIX invView = XMMatrixInverse(nullptr, _viewmatrix);
	for (int i = 0; i < 8; i++) {
		corners[i] = XMVector3TransformCoord(corners[i], invView);
	}

	return corners;
}

bool Camera::ChackView(const XMFLOAT3& pos, const XMFLOAT3& scl) {
	if (Manager::GetisDrawFromLight()) {
		return true;
	}
	const float& rad = std::max(std::max(scl.x, scl.y), scl.z);

	XMMATRIX vp, invvp;
	XMVECTOR det;

	vp = _viewmatrix * _projectionMatrix;
	invvp = XMMatrixInverse(&det, vp);


	XMFLOAT3 vpos[4];
	XMFLOAT3 wpos[4];

	vpos[0] = { -1.0f, 1.0f, 1.0f };
	vpos[1] = { 1.0f, 1.0f, 1.0f };
	vpos[2] = { -1.0f, -1.0f, 1.0f };
	vpos[3] = { 1.0f, -1.0f, 1.0f };

	XMVECTOR vposv[4];
	XMVECTOR wposv[4];

	for (int i = 0; i < 4; i++) {
		vposv[i] = XMLoadFloat3(&vpos[i]);

		wposv[i] = XMVector3TransformCoord(vposv[i], invvp);

		XMStoreFloat3(&wpos[i], wposv[i]);
	}

	XMFLOAT3 v, v1, v2, n;
	v = pos - GetPosition();

	//���ʔ���
	{
		XMFLOAT3 forward;
		XMStoreFloat3(&forward, GetZDirection());
		forward = VectorNormalize(forward);

		//���ςő��ʂ���̋���
		float dotF = forward.x * v.x + forward.y * v.y + forward.z * v.z;

		if (dotF > FAR_CLIP) {
			return false;
		}
	}
	//���ʔ���
	{
		v1 = wpos[0] - GetPosition();
		v2 = wpos[2] - GetPosition();

		//�O�ςŖ@�������߂�
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//���K��
		n = VectorNormalize(n);

		//���ςő��ʂ���̋���
		float dotL = n.x * v.x + n.y * v.y + n.z * v.z;

		if (dotL < -rad) {	//0.0f��-���a��
			return false;
		}
	}
	//�E�ʔ���
	{
		v1 = wpos[3] - GetPosition();
		v2 = wpos[1] - GetPosition();

		//�O�ςŖ@�������߂�
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//���K��
		n = VectorNormalize(n);

		//���ςő��ʂ���̋���
		float dotR = n.x * v.x + n.y * v.y + n.z * v.z;

		if (dotR < -rad) {
			return false;
		}
	}
	//��ʔ���
	{
		v1 = wpos[1] - GetPosition();
		v2 = wpos[0] - GetPosition();

		//�O�ςŖ@�������߂�
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//���K��
		n = VectorNormalize(n);

		//���ςő��ʂ���̋���
		float dotT = n.x * v.x + n.y * v.y + n.z * v.z;

		if (dotT < -rad) {
			return false;
		}
	}
	//���ʔ���
	{
		v1 = wpos[2] - GetPosition();
		v2 = wpos[3] - GetPosition();

		//�O�ςŖ@�������߂�
		n.x = v1.y * v2.z - v1.z * v2.y;
		n.y = v1.z * v2.x - v1.x * v2.z;
		n.z = v1.x * v2.y - v1.y * v2.x;

		//���K��
		n = VectorNormalize(n);

		//���ςő��ʂ���̋���
		float dotB = n.x * v.x + n.y * v.y + n.z * v.z;

		if (dotB < -rad) {
			return false;
		}
	}

	return true;
}

std::vector<int> Camera::CheckViewInstance(const XMFLOAT3* pos, const XMFLOAT3 basepos, const int& instancenum, const XMFLOAT3& scl) {
	std::vector<int> result;

	for (int i = 0; i < instancenum; i++) {
		if (ChackView(pos[i] + basepos, scl)) {
			result.push_back(i);
		}
	}
	
	return result;
}

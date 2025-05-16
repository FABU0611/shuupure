//Transform.h
//20106_田中　蓮
//24_05_22
#pragma once

#include "Main.h"
#include "Component.h"

class Transform : public Component {
	bool		_isusequaternion = false;

public:
	using Component::Component;

	XMFLOAT3	_position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3	_rotation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3	_scale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT4	_quaternion = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT3	_forward = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3	_up = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3	_right = { 0.0f, 0.0f, 0.0f };

    void Update()override {
        XMMATRIX rotationMatrix;

		//オイラー角でマトリックスを作る
		if (!_isusequaternion) {
			rotationMatrix = XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
		}
		//クォータニオンでマトリックスを作る
		else {
			rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&_quaternion));
		}

		//方向ベクトル更新
        XMStoreFloat3(&_forward, rotationMatrix.r[2]);
        XMStoreFloat3(&_up, rotationMatrix.r[1]);
        XMStoreFloat3(&_right, rotationMatrix.r[0]);
    }

	void UseQuaternion() { _isusequaternion = true; }
};
//Player_Tutorial.h
//20106_ìcíÜÅ@ò@
//24_09_17
#pragma once

#include "Player.h"
#include "Time.h"

class Player_Tutorial : public Player {
	ID3D11VertexShader*	_VertexShader;
	ID3D11PixelShader*	_PixelShader;
	ID3D11InputLayout*	_VertexLayout;

	XMFLOAT3	_angulvel{};
	const float	_friction = 0.98f;

public:
	Player_Tutorial(const XMFLOAT3& pos, const XMFLOAT3& scl);
	~Player_Tutorial() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Hit(GameObject* obj);

	void TrunHeadup() {
		XMFLOAT4 headup = { 0.0f, 0.0f, 0.0f, 1.0f };
		
		XMVECTOR newq = XMQuaternionSlerp(XMLoadFloat4(&GetQuaternion()), XMLoadFloat4(&headup), Time::GetDeltaTime() * 2.0f);
		XMStoreFloat4(&GetQuaternion(), newq);
	}
};
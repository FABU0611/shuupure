//PlayerState_underwater.cpp
//20106_�c���@�@
//24_08_28
#include "PlayerState_underwater.h"
#include "Player.h"
#include "Time.h"
#include "PlayerState_end.h"

void PlayerStateUnderwater::Update(){
	float depth = 0.0f - GetPlayer()->GetPosition().y;
	//���͂̌v�Z
	float buoyancy = 9.8f * depth;

	//�����͂̓K�p
	GetPlayer()->GetVelocity().y += (buoyancy - (GetPlayer()->GetAccel().y * 0.3f)) * Time::GetDeltaTime();
	GetPlayer()->GetVelocity().y *= 0.966f;


	//��������
	GetPlayer()->Fall(Time::GetDeltaTime());

	// ���ݍ��ވʒu�̉�����ݒ�
	if (GetPlayer()->GetPosition().y < -15.0f) {
		// ����ȏ㒾�܂Ȃ��悤�ɂ���
		GetPlayer()->GetPosition().y = -15.0f;
		GetPlayer()->GetVelocity().y = 0.0f;  // ���x���[���ɂ���
	}

	//������ɂ���
	GetPlayer()->TrunHeadup();

	//���ʂ܂ŗ�����
	if (GetPlayer()->GetPosition().y >= 0.0f) {
		GetPlayer()->GetPosition().y = 0.0f;
		GetPlayer()->GetVelocity().y = 0.0f;
		GetPlayer()->ChangeState(new PlayerStateEnd(GetPlayer())); // ���S�ɕ��������
	}
}

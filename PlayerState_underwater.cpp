//PlayerState_underwater.cpp
//20106_田中　蓮
//24_08_28
#include "PlayerState_underwater.h"
#include "Player.h"
#include "Time.h"
#include "PlayerState_end.h"

void PlayerStateUnderwater::Update(){
	float depth = 0.0f - GetPlayer()->GetPosition().y;
	//浮力の計算
	float buoyancy = 9.8f * depth;

	//減衰力の適用
	GetPlayer()->GetVelocity().y += (buoyancy - (GetPlayer()->GetAccel().y * 0.3f)) * Time::GetDeltaTime();
	GetPlayer()->GetVelocity().y *= 0.966f;


	//落下処理
	GetPlayer()->Fall(Time::GetDeltaTime());

	// 沈み込む位置の下限を設定
	if (GetPlayer()->GetPosition().y < -15.0f) {
		// これ以上沈まないようにする
		GetPlayer()->GetPosition().y = -15.0f;
		GetPlayer()->GetVelocity().y = 0.0f;  // 速度をゼロにする
	}

	//頭を上にする
	GetPlayer()->TrunHeadup();

	//水面まで来たら
	if (GetPlayer()->GetPosition().y >= 0.0f) {
		GetPlayer()->GetPosition().y = 0.0f;
		GetPlayer()->GetVelocity().y = 0.0f;
		GetPlayer()->ChangeState(new PlayerStateEnd(GetPlayer())); // 完全に浮いた状態
	}
}

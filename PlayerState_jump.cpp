//PlayerState_jump.h
//20106_田中　蓮
//24_08_28
#include "PlayerState_jump.h"
#include "Manager.h"
#include "Player.h"
#include "PlayerState_rorate.h"
#include "Input.h"
#include "ParticleJump.h"

void PlayerStateJump::Update() {
	//ジャンプ力チャージ
	if (Input::GetKeyPress(VK_SPACE)) {
		_jump += 1.0f;
		if (_jump > 100.0f) {
			_jump = 100.0f;
			_chage = true;
		}
	}
	//キーを離してジャンプ
	if (Input::GetKeyRelease(VK_SPACE) ||
		Input::GetButtonTrigger(0, XINPUT_GAMEPAD_A)) {
		Scene* scene;
		scene = Manager::GetScene();
		//エフェクトを出す
		scene->AddGameobject<Shockwave>(Effect, GetPlayer()->GetPosition(), 0.7f, XMFLOAT4(0.1f, 0.5f, 0.5f, 1.0f));
		
		GetPlayer()->PlayAudioJump();
		GetPlayer()->SetVelocity({ 75.0f, _jump, 0.0f });
		GetPlayer()->ChangeState(new PlayerStateRotate(GetPlayer()));
	}
}

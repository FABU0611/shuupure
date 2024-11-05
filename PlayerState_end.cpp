//PlayerState_end.cpp
//20106_“c’†@˜@
//24_08_28
#include "PlayerState_end.h"
#include "Player.h"
#include "Time.h"
#include "Manager.h"
#include "S_Result.h"

void PlayerStateEnd::Update(){
	//“ª‚ğã‚É‚·‚é
	GetPlayer()->TrunHeadup();

	//ã‰º‚É‚Õ‚©‚Õ‚©‚·‚é
	_time += Time::GetDeltaTime();
	GetPlayer()->GetPosition().y -= sinf(_time * 1.0f * XM_2PI) * 1.0f + GetPlayer()->GetPosition().y;
	
	//5•bŒã‚ÉƒŠƒUƒ‹ƒg‚É‘JˆÚ
	if (_time > 5.0f) {
		Manager::SetSceneFade <Result>(0.01f);
	}
}

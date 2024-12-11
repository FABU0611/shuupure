//TitlePush.cpp
//20106_田中　蓮
//24_09_15
#include "T_PushStart.h"
#include "Manager.h"
#include "Renderer.h"
#include "S_Title.h"
#include "Input.h"
#include "C_Audio.h"
#include "EasingFunc.h"
#include "Time.h"
#include "G_Button.h"
#include "GUIManager.h"

void TitlePush::Init(){
	Text::Init();

	AddComponent<Audio>(this);
	GetComponent<Audio>()->Load("asset\\audio\\push.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\push.wav", 0.5f);
	GetComponent<Audio>()->Load("asset\\audio\\bgm002.wav");
	GetComponent<Audio>()->SetVolume("asset\\audio\\bgm002.wav", 0.2f);
	GetComponent<Audio>()->Play("asset\\audio\\bgm002.wav", false);

	_spos.x = _epos.x = GetPosition().x = (SCREEN_WIDTH * 0.5f);
	_spos.y = SCREEN_HEIGHT + 100.0f;
	_epos.y = 600.0f;

	_start = Manager::GetGUIManager()->AddGUI<Button>(XMFLOAT3((SCREEN_WIDTH * 0.5f), 100.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f), L"asset\\texture\\particle.png");

	_str = "Push K key";
}

void TitlePush::Update(){
	//フォントのアニメーション
	float t = _time;
	_time += Time::GetDeltaTime();
	GetPosition().y = std::lerp(_spos.y, _epos.y, Easing::EaseOutBounce(t));

	if (t > 1.0f) {
		_time = 1.0f;
		GetPosition().y = _epos.y;
	}
	Button* button = dynamic_cast<Button*>(Manager::GetGUIManager()->GetGUI(_start));
	if (!button) {
		return;
	}
	//シーン遷移
	if (Input::GetKeyTrigger('K') || button->OnClicked(VK_LBUTTON)) {
		GetComponent<Audio>()->Play("asset\\audio\\push.wav", false);
		Manager::SetSceneFade<Title>(0.05f);
	}
}

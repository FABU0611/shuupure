//TitlePush.cpp
//20106_ìcíÜÅ@ò@
//24_09_15
#include "T_PushStart.h"
#include "Manager.h"
#include "Renderer.h"
#include "S_Game.h"
#include "S_Tutorial.h"
#include "Input.h"
#include "C_Audio.h"
#include "EasingFunc.h"
#include "Time.h"
#include "G_Button.h"
#include "GUIManager.h"

void TitlePush::Init(){
	Text::Init();

	AddComponent<Audio>(this);
	//GetComponent<Audio>()->Load("asset\\audio\\push.wav");
	//GetComponent<Audio>()->SetVolume("asset\\audio\\push.wav", 0.5f);
	//GetComponent<Audio>()->Load("asset\\audio\\bgm002.wav");
	//GetComponent<Audio>()->SetVolume("asset\\audio\\bgm002.wav", 0.2f);
	//GetComponent<Audio>()->Play("asset\\audio\\bgm002.wav", false);

	SetPosition({ SCREEN_WIDTH * 0.5f, 600.0f, 0.0f });

	_start = Manager::GetGUIManager()->AddGUI<Button>(GetPosition(), XMFLOAT3(300.0f, 100.0f, 100.0f), L"asset\\texture\\button.dds");

	_str = "Start";
}

void TitlePush::Update(){
	Button* button = dynamic_cast<Button*>(Manager::GetGUIManager()->GetGUI(_start));
	if (!button) {
		return;
	}
	//ÉVÅ[ÉìëJà⁄
	if (button->OnClicked(VK_LBUTTON)) {
		GetComponent<Audio>()->Play("asset\\audio\\push.wav", false);
		Manager::SetSceneFade<Game>(0.05f);
	}
}

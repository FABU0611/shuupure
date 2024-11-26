//S_Title.cpp	タイトルシーン
//20106_田中　蓮
//24_07_03
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "TextManager.h"

#include "S_Title.h"
#include "S_Tutorial.h"

#include "Polygon2D.h"
#include "T_Pushstart.h"
#include "T_Title.h"

#include "Input.h"

void Title::Init(){
	AddGameobject<Polygon2D>(BackGround, XMFLOAT3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), L"asset\\texture\\title_back.png");

	TextManager* tm = Manager::GetTextManager();
	tm->AddText<TitlePush>(60.0f, D2D1::ColorF::Yellow);
	tm->AddText<TitleText>(120.0f, D2D1::ColorF::GreenYellow);
}

void Title::Update() {
	Scene::Update();
	if (Input::GetKeyTrigger('K')) {
		Manager::SetSceneFade<Title>(0.05f);
	}
}

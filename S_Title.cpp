//S_Title.cpp	タイトルシーン
//20106_田中　蓮
//24_07_03
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "TextManager.h"
#include "GUIManager.h"

#include "S_Title.h"
#include "S_Tutorial.h"

#include "Camera.h"
#include "LightCamera.h"
#include "Wave.h"
#include "Sky.h"
#include "MeshField.h"
#include "Player.h"
#include "Pool.h"
#include "DivingBoard.h"
#include "Polygon2D.h"
#include "T_Title.h"
#include "G_Button.h"
#include "S_Game.h"

void Title::Init(){
	AddGameobject<Camera>(Layer::System);
	AddGameobject<LightCamera>(Layer::System);

	AddGameobject<MeshField>(Layer::Object);
	AddGameobject<Player>(Layer::Object, XMFLOAT3(35.0f, 7.0f, 5.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

	AddGameobject<Wave>(Layer::Transparency);

	AddGameobject<Sky>(Layer::BackGround, 250.0f);

	_start = GUIManager::GetInstance()->AddGUI<Button>("Start", D2D1::ColorF::Yellow, XMFLOAT3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f), XMFLOAT3(300.0f, 100.0f, 100.0f));
	TextManager::GetInstance()->AddText<TitleText>(80.0f, D2D1::ColorF::GreenYellow, TextAnchor::Center);
}

void Title::Update() {
	Button* button = dynamic_cast<Button*>(GUIManager::GetInstance()->GetGUI(_start));
	if (!button) {
		return;
	}
	//シーン遷移
	if (button->OnClicked(VK_LBUTTON)) {
		Manager::SetSceneFade<Game>(0.05f);
	}

	Scene::Update();
}

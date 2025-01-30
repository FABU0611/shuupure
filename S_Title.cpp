//S_Title.cpp	タイトルシーン
//20106_田中　蓮
//24_07_03
#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "TextManager.h"

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
#include "T_Pushstart.h"
#include "T_Title.h"

void Title::Init(){
	AddGameobject<Camera>(Layer::System);
	AddGameobject<LightCamera>(Layer::System);

	AddGameobject<MeshField>(Layer::Object);
	AddGameobject<Player>(Layer::Object, XMFLOAT3(35.0f, 7.0f, 5.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	//AddGameobject<Pool>(Layer::Object, XMFLOAT3(0.0f, 0.0f, 0.0f));
	//AddGameobject<DivingBoard>(Layer::Object, XMFLOAT3(-30.0f, 0.0f, 0.0f));

	AddGameobject<Wave>(Layer::Transparency);

	AddGameobject<Sky>(Layer::BackGround, 250.0f);

	TextManager* tm = Manager::GetTextManager();
	tm->AddText<TitlePush>(60.0f, D2D1::ColorF::Yellow, TextAnchor::Center);
	tm->AddText<TitleText>(120.0f, D2D1::ColorF::GreenYellow, TextAnchor::Center);
}
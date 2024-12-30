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
	_light = new LIGHT();

	_light->Enable = true;
	_light->Direction = XMFLOAT4(1.0f, -1.0f, 0.5f, 0.0f);
	_light->Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	_light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	_light->Position = XMFLOAT4(0.0f, 10.0f, 20.0f, 0.0f);
	_light->PointLightParam = XMFLOAT4(5000.0f, 0.0f, 0.0f, 0.0f);

	AddGameobject<Camera>(Layer::System);

	//AddGameobject<MeshField>(Object);
	AddGameobject<Player>(Layer::Object, XMFLOAT3(35.0f, 7.0f, 5.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	AddGameobject<Pool>(Layer::Object, XMFLOAT3(0.0f, 0.0f, 0.0f));
	AddGameobject<DivingBoard>(Layer::Object, XMFLOAT3(-30.0f, 0.0f, 0.0f));

	//AddGameobject<Wave>(Transparency);

	//AddGameobject<Sky>(BackGround, 100.0f);

	TextManager* tm = Manager::GetTextManager();
	tm->AddText<TitlePush>(60.0f, D2D1::ColorF::Yellow, TextAnchor::Center);
	tm->AddText<TitleText>(120.0f, D2D1::ColorF::GreenYellow, TextAnchor::Center);
}
void Title::Uninit() {
	delete _light;

	Scene::Uninit();
}

void Title::Update() {
	//水面のアニメーション用
	_light->PointLightParam.y += 0.01f;
	_light->PointLightParam.z += 0.01f;

	Scene::Update();
}


void Title::Draw() {
	//Renderer::SetLight((*_light));

	Scene::Draw();
}
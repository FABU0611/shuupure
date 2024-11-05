//S_Tutorial.cpp	チュートリアルシーン
//20106_田中　蓮
//24_09_17
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"

#include "S_Game.h"
#include "S_Tutorial.h"

#include "Input.h"
#include "Camera.h"
#include "Player_Tutorial.h"
#include "Skyobj.h"
#include "TutorialText.h"

void Tutorial::Init() {
	_light = new LIGHT();

	_light->Enable = true;
	_light->Direction = XMFLOAT4(-1.0f, -1.0f, -0.5f, 0.0f);
	_light->Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	_light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	_light->Position = XMFLOAT4(0.0f, 10.0f, 20.0f, 0.0f);
	_light->PointLightParam = XMFLOAT4(5000.0f, 0.0f, 0.0f, 0.0f);

	AddGameobject<Camera>(System);

	AddGameobject<Skyobj>(Skydoom, 300.0f);

	AddGameobject<Player_Tutorial>(Object, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));

	AddGameobject<TutorialText>(UI);
}

void Tutorial::Uninit(){
	delete _light;

	Scene::Uninit();
}

void Tutorial::Update(){
	Scene::Update();
	if (Input::GetKeyTrigger(VK_SPACE)) {
		Manager::SetSceneFade<Game>(0.05f);
	}
}

void Tutorial::Draw(){
	Renderer::SetLight((*_light));

	Scene::Draw();
}

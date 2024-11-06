//S_Game.cpp	ゲームシーン
//20106_田中　蓮
//24_07_03
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"

#include "S_Game.h"
#include "S_Result.h"

#include "Camera.h"
#include "Player.h"
#include "Sky.h"
#include "ParticleFireup.h"
#include "DivingBoard.h"
#include "Pool.h"
#include "Water.h"
#include "UIManager.h"

void Game::Init() {
	_light = new LIGHT();

	_light->Enable = true;
	_light->Direction = XMFLOAT4(-1.0f, -1.0f, -0.5f, 0.0f);
	_light->Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	_light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	_light->Position = XMFLOAT4(0.0f, 10.0f, 20.0f, 0.0f);
	_light->PointLightParam = XMFLOAT4(5000.0f, 0.0f, 0.0f, 0.0f);

	AddGameobject<Camera>(System);

	AddGameobject<Sky>(Skydoom, 300.0f);

	AddGameobject<Player>(Object, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	AddGameobject<DivingBoard>(Object, XMFLOAT3(-30.0f, 0.0f, 0.0f));
	AddGameobject<Pool>(Object, XMFLOAT3(0.0f, 0.0f, 0.0f));
	AddGameobject<Water>(Effect, XMFLOAT3(40.0f, 20.0f, 0.0f));

	AddGameobject<UIManager>(UI);
}

void Game::Uninit(){
	delete _light;

	Scene::Uninit();
}

void Game::Update(){
	//水面のアニメーション用
	_light->PointLightParam.y += 0.01f;
	_light->PointLightParam.z += 0.01f;

	Scene::Update();
}

void Game::Draw(){
	Renderer::SetLight((*_light));

	Scene::Draw();
}

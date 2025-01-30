//S_Game.cpp	ゲームシーン
//20106_田中　蓮
//24_07_03
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"

#include "S_Game.h"
#include "S_Result.h"

#include "Camera.h"
#include "LightCamera.h"
#include "Player.h"
#include "Sky.h"
#include "ParticleFireup.h"
#include "DivingBoard.h"
#include "Pool.h"
#include "TestObject.h"
#include "SphereInstence.h"
#include "Water.h"
#include "UIManager.h"

void Game::Init() {
	AddGameobject<Camera>(Layer::System);
	AddGameobject<LightCamera>(Layer::System);

	AddGameobject<Sky>(Layer::BackGround, 250.0f);

	AddGameobject<Player>(Layer::Object, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	AddGameobject<Pool>(Layer::Object, XMFLOAT3(0.0f, 0.0f, 0.0f));
	AddGameobject<DivingBoard>(Layer::Object, XMFLOAT3(-30.0f, 0.0f, 0.0f));
	AddGameobject<TestObject>(Layer::Object, XMFLOAT3(50.0f, 10.0f, 30.0f));
	AddGameobject<SphereInstance>(Layer::Object);
	//AddGameobject<Water>(Layer::Effect, XMFLOAT3(40.0f, 20.0f, 0.0f));
}
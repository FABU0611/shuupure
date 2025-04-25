//S_Game.cpp	ゲームシーン
//20106_田中　蓮
//24_07_03
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"

#include "S_Game.h"
#include "S_Result.h"

#include "Input.h"
#include "Camera.h"
#include "LightCamera.h"
#include "Player.h"
#include "Sky.h"
#include "ParticleFireup.h"
#include "DivingBoard.h"
#include "Pool.h"
#include "TestObject.h"
#include "PBRObject.h"
#include "SphereInstence.h"
#include "Water.h"
#include "UIManager.h"
#include "CheckTexture.h"

void Game::Init() {
	AddGameobject<Camera>(Layer::System);
	AddGameobject<LightCamera>(Layer::System);

	//AddGameobject<Sky>(Layer::BackGround, 250.0f);

	//AddGameobject<Player>(Layer::Object, XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	//AddGameobject<Pool>(Layer::Object, XMFLOAT3(0.0f, 0.0f, 0.0f));
	AddGameobject<TestObject>(Layer::Object, XMFLOAT3(-10.0f, 10.0f, 30.0f));
	AddGameobject<PBRObject>(Layer::Object);
	AddGameobject<SphereInstance>(Layer::Object);
	//AddGameobject<Water>(Layer::Transparency, XMFLOAT3(40.0f, 20.0f, 0.0f));
	//AddGameobject<CheckTexture>(Layer::UI, "DoF", Renderer::GetDepthTexture(), ShaderName::CheckDoF, XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
}
void Game::Update() {
	//シーン遷移
	if (Input::GetKeyTrigger(VK_SPACE)) {
		Manager::SetSceneFade<Game>(0.05f);
	}

	Scene::Update();
}

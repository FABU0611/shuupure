//S_Result.cpp	���U���g�V�[��
//20106_�c���@�@
//24_07_03
#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Input.h"

#include "S_Result.h"
#include "S_Title.h"

#include "Polygon2D.h"
#include "ScoreText.h"

void Result::Init(){
	AddGameobject<Polygon2D>(UI, XMFLOAT3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), L"asset\\texture\\result_back.png");
	AddGameobject<ScoreText>(UI);
}

void Result::Update(){
	Scene::Update();
	//�V�[���J��
	if (Input::GetKeyTrigger('K') ||
		Input::GetButtonTrigger(0, XINPUT_GAMEPAD_START)) {
		Manager::SetSceneFade<Title>(0.1f);
	}
}

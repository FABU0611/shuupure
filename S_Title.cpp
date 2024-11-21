//S_Title.cpp	タイトルシーン
//20106_田中　蓮
//24_07_03
#include "Main.h"
#include "Renderer.h"

#include "S_Title.h"

#include "Polygon2D.h"
#include "T_Pushstart.h"
#include "T_Title.h"

void Title::Init(){
	AddGameobject<Polygon2D>(UI, XMFLOAT3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), L"asset\\texture\\title_back.png");
	AddGameobject<TitlePush>(UI);
	AddGameobject<TitleText>(UI);
}

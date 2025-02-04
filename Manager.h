#pragma once


#include "Scene.h"
#include "C_Sprite2D.h"
#include "DirectWriteCustomFont.h"
#include <string>
#include "Fade.h"

class Rendpoly;
class Gaussian;
class MotionBlur;
class LoadingText;
class GUIManager;
class TextManager;
//#ifdef _DEBUG
class CheckDoF;
class CheckCameraDepth;
//#endif


class Manager {
	static Scene* _scene;
	static Scene* _nextscene;
	static Scene* _prvscene;

	static LoadingText* _loadingT;

	static Fade* _fade;

	static Rendpoly* _final;

	static Gaussian* _gaussian;

	static MotionBlur* _motionblur;

	static GUIManager* _guimanager;

	static TextManager* _textmanager;
//#ifdef _DEBUG
	static CheckDoF* _checkdof;
	static CheckCameraDepth* _checkcamera;
//#endif

	static bool _isdrawfromlight;

	static int	_cascadeidx;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void FadeUpdate();

	static bool& GetisDrawFromLight() { return _isdrawfromlight; }

	static Scene* GetScene() { return _scene; }
	static TextManager* GetTextManager() { return _textmanager; }
	static Gaussian* GetGaussian() { return _gaussian; }
	static int& GetCascadeIndex() { return _cascadeidx; }

	static void ChangeNextScene() {
		_nextscene = _prvscene;
		_prvscene = nullptr;
	}

	static GUIManager* GetGUIManager() { return _guimanager; }
	
	template <typename T>
	static void SetScene() { 
		_nextscene = new T();
	}
	template <typename T>
	static void SetSceneFade(const float& rate) { 
		_prvscene = new T();
		_fade->SetFaderate(rate);
		_fade->GetFadeMode() = FadeMode::Out;
	}
};
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
#ifdef _DEBUG
class CheckDoF;
#endif


class Manager{
	static Scene*	_scene;
	static Scene*	_nextscene;
	static Scene*	_prvscene;

	static LoadingText*	_loadingT;

	static Fade*		_fade;

	static Rendpoly*	_final;

	static Gaussian*	_gaussian;

	static MotionBlur*	_motionblur;
#ifdef _DEBUG
	static CheckDoF*	_checkdof;
#endif

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void FadeUpdate();

	static Scene* GetScene() { return _scene; }

	static void ChangeNextScene() {
		_nextscene = _prvscene;
		_prvscene = nullptr;
	}
	
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
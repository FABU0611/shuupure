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
class TakePicture;


class Manager {
	static float	WORLD_RAD;

	static Scene* _scene;
	static Scene* _nextscene;
	static Scene* _prvscene;

	static LoadingText* _loadingT;

	static Fade* _fade;

	static Rendpoly* _final;

	static Gaussian* _gaussian;

	static TakePicture* _takepic;

	static bool _isdrawfromlight;

	static int	_cascadeidx;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void FadeUpdate();

	const static float GetWorldRad() { return WORLD_RAD; }
	static bool& GetisDrawFromLight() { return _isdrawfromlight; }

	static Scene* GetScene() { return _scene; }
	static Gaussian* GetGaussian() { return _gaussian; }
	static int& GetCascadeIndex() { return _cascadeidx; }

	static void ChangeNextScene() {
		_nextscene = _prvscene;
		_prvscene = nullptr;
	}	

	static void SetWorldRad(const float& worldrad) { WORLD_RAD = worldrad; }

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
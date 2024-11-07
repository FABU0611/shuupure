#pragma once


#include "Scene.h"
#include "C_Sprite2D.h"
#include "DirectWriteCustomFont.h"
#include <string>

class Fade;
class Rendpoly;
class Gaussian;
#ifdef _DEBUG
class CheckDoF;
#endif

enum class FadeMode {
	None,
	In,
	Out,
};

class Manager{
	static Scene*	_scene;
	static Scene*	_nextscene;
	static Scene*	_prvscene;

	static Sprite2D*	_sprite;
	static FadeMode		_mode;
	static XMFLOAT4		_fadecolor;
	static float		_faderate;

	static FontData*	_fontdata;
	static DirectWriteCustomFont* _loadT;

	static std::string	_loading;
	static float		_time;

	static Fade*		_fade;

	static Rendpoly*	_final;

	static Gaussian*	_gaussian;
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
	
	template <typename T>
	static void SetScene() { 
		_nextscene = new T();
	}
	template <typename T>
	static void SetSceneFade(const float& rate) { 
		_prvscene = new T();
		_faderate = rate;
		_mode = FadeMode::Out;
	}
};
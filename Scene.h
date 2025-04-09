//Scene.h	シーン
//20106_田中　蓮
//24_05_29
#pragma once
#include "GameObject.h"
#include <list>

enum class Layer{
	System,
	BackGround,
	Object,
	Transparency,
	Effect,
	UI,

	MAX_LAYER
};

class Scene {
protected:
	std::list<GameObject*>	_gameobjects[static_cast<int>(Layer::MAX_LAYER)];

public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() {};
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void DrawFromLight();
	virtual void DrawUI();

	template <typename T, typename... Args>
	T* AddGameobject(const Layer& layer, Args&& ...args) {
		T* gameobject = new T(std::forward<Args>(args)...);		//Memo:newはCPU負荷が高くなるので自作newする。plasement new。https://logicalbeat.jp/blog/5016/ https://github.com/microsoft/mimalloc
		_gameobjects[static_cast<int>(layer)].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	//オブジェクトを取得
	template <typename T>
	T* GetGameobject() {
		for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
			for (auto o : _gameobjects[i]) {
				T* ret = dynamic_cast<T*>(o);

				if (ret != nullptr) {
					return ret;
				}
			}
		}
		return nullptr;
	}

	//複数のオブジェクトを取得
	template <typename T>
	std::vector<T*> GetGameobjects() {
		std::vector<T*> objectlist;
		for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
			for (auto o : _gameobjects[i]) {
				T* ret = dynamic_cast<T*>(o);

				if (ret != nullptr) {
					objectlist.push_back(ret);
				}
			}
		}
		return objectlist;
	}

	//当たり判定処理
	void CheckHit(GameObject* obj);
	void ZSort();
};
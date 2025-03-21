//Scene.h	シーン
//20106_田中　蓮
//24_05_29
#pragma once
#include "GameObject.h"
#include <list>
#include <algorithm>

#include "Camera.h"
#include "LightCamera.h"
#include "Sky.h"
#include "C_Collision.h"

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
	virtual void Uninit() {
		for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
			for (auto o : _gameobjects[i]) {
				o->Uninit();
				delete o;
				o = nullptr;
			}
			_gameobjects[i].clear();
		}
	};
	virtual void Update() {
		for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
			for (auto o : _gameobjects[i]) {
				//オブジェクトの更新処理
				o->Update();
				//当たり判定処理
				CheckHit(o);
			}

			//いらなくなったものを削除
			_gameobjects[i].remove_if([](GameObject* gameobject) {
				return gameobject->Destroy(); }
			);
		}
	};
	virtual void Draw() {
		ZSort();

		for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
			for (auto o : _gameobjects[i]) {
				if (i == static_cast<int>(Layer::System)) {
					LightCamera* lightcamera = dynamic_cast<LightCamera*>(o);
					if (lightcamera) {
						continue;
					}
				}
				//描画処理
				o->Draw();
			}
		}
	};
	virtual void DrawFromLight() {
		ZSort();

		for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
			for (auto o : _gameobjects[i]) {
				if (i == static_cast<int>(Layer::System)) {
					Camera* camera = dynamic_cast<Camera*>(o);
					if (camera) {
						continue;
					}
				}
				else if (i == static_cast<int>(Layer::BackGround)) {
					Sky* sky = dynamic_cast<Sky*>(o);
					if (sky) {
						continue;
					}
				}
				else if (i == static_cast<int>(Layer::Transparency)) {
					continue;
				}
				//描画処理
				o->Draw();
			}
		}
	}

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
	void CheckHit(GameObject* obj) {
		for (auto o : _gameobjects[static_cast<int>(Layer::Object)]) {
			//コリジョンを持っているものだけ処理
			if (!obj->GetComponent<Collision>() ||
				!o->GetComponent<Collision>()) {
				continue;
			}
			//同じオブジェクトなら処理しない
			if (o == obj) {
				continue;
			}
			//コリジョンだけ抽出
			Collision* col1 = obj->GetComponent<Collision>();
			Collision* col2 = o->GetComponent<Collision>();

			//当たり判定処理
			if (Collision::CheckHit(col1, col2)) {
				obj->Hit(o);
			}
		}
	}

	void ZSort() {
		//カメラからの距離の降順でソート
		Camera* c = GetGameobject<Camera>();
		if (!c) {
			return;
		}
		//カメラのポジションをベクター型に変換
		XMVECTOR camP = XMLoadFloat3(&c->GetPosition());

		//カメラのZベクトルを取得
		XMVECTOR camD = c->GetZDirection();
		//正規化
		XMVector3Normalize(camD);

		for (auto& o : _gameobjects[static_cast<int>(Layer::Object)]) {
			XMVECTOR pos = XMLoadFloat3(&o->GetPosition());
			XMVECTOR dir = pos - camP;
			o->SetCamDistance(XMVectorGetX(XMVector3Dot(dir, camD)));
		}

		//Zソート
		_gameobjects[static_cast<int>(Layer::Object)].sort([](GameObject* a, GameObject* b) {
			return (a->GetCamDistance() > b->GetCamDistance());
			});
		//射影、当たり判定でも使う
	}
};
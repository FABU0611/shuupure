//Scene.cpp
//20106_田中　蓮
//25_04_07
#include "Scene.h"

#include "Camera.h"
#include "LightCamera.h"
#include "Sky.h"
#include "C_Collision.h"
#include "TextManager.h"
#include "GUIManager.h"

void Scene::Uninit() {
	for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
		for (auto o : _gameobjects[i]) {
			o->Uninit();
			delete o;
			o = nullptr;
		}
		_gameobjects[i].clear();
	}
}

void Scene::Update() {
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
}

void Scene::Draw() {
	ZSort();

	for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
		for (auto o : _gameobjects[i]) {
			if (i == static_cast<int>(Layer::System)) {
				LightCamera* lightcamera = dynamic_cast<LightCamera*>(o);
				if (lightcamera) {
					continue;
				}
			}
			else if (i == static_cast<int>(Layer::UI)) {
				continue;
			}
			//描画処理
			o->Draw();
		}
	}
}

void Scene::DrawFromLight() {
	ZSort();

	for (int i = 0; i < static_cast<int>(Layer::MAX_LAYER); i++) {
		if (i == static_cast<int>(Layer::Transparency) ||
			i == static_cast<int>(Layer::UI)) {
			continue;
		}
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
			//描画処理
			o->Draw();
		}
	}
}

void Scene::DrawUI() {
	GUIManager::GetInstance()->Draw();
	TextManager::GetInstance()->Draw();
	for (auto o : _gameobjects[static_cast<int>(Layer::UI)]) {
		o->Draw();
	}
}

void Scene::CheckHit(GameObject* obj) {
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

void Scene::ZSort() {
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

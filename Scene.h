//Scene.h	�V�[��
//20106_�c���@�@
//24_05_29
#pragma once
#include "GameObject.h"
#include <list>
#include <algorithm>

#include "Camera.h"
#include "C_Collision.h"

enum Layer{
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
	std::list<GameObject*>	_gameobjects[MAX_LAYER];

public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() {};
	virtual void Uninit() {
		for (int i = 0; i < MAX_LAYER; i++) {
			for (auto o : _gameobjects[i]) {
				o->Uninit();
				delete o;
				o = nullptr;
			}
			_gameobjects[i].clear();
		}
	};
	virtual void Update() {
		for (int i = 0; i < MAX_LAYER; i++) {
			for (auto o : _gameobjects[i]) {
				//�I�u�W�F�N�g�̍X�V����
				o->Update();
				//�����蔻�菈��
				CheckHit(o);
			}

			//����Ȃ��Ȃ������̂��폜
			_gameobjects[i].remove_if([](GameObject* gameobject) {
				return gameobject->Destroy(); }
			);
		}
	};
	virtual void Draw() {
		ZSort();

		for (int i = 0; i < MAX_LAYER; i++) {
			for (auto o : _gameobjects[i]) {
				//�`�揈��
				o->Draw();
			}
		}
	};

	template <typename T, typename... Args>
	T* AddGameobject(const int& layer, Args&& ...args) {
		T* gameobject = new T(std::forward<Args>(args)...);
		_gameobjects[layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	//�I�u�W�F�N�g���擾
	template <typename T>
	T* GetGameobject() {
		for (int i = 0; i < MAX_LAYER; i++) {
			for (auto o : _gameobjects[i]) {
				T* ret = dynamic_cast<T*>(o);

				if (ret != nullptr) {
					return ret;
				}
			}
		}
		return nullptr;
	}

	//�����̃I�u�W�F�N�g���擾
	template <typename T>
	std::vector<T*> GetGameobjects() {
		std::vector<T*> objectlist;
		for (int i = 0; i < MAX_LAYER; i++) {
			for (auto o : _gameobjects[i]) {
				T* ret = dynamic_cast<T*>(o);

				if (ret != nullptr) {
					objectlist.push_back(ret);
				}
			}
		}
		return objectlist;
	}

	//�����蔻�菈��
	void CheckHit(GameObject* obj) {
		for (auto o : _gameobjects[Object]) {
			//�R���W�����������Ă�����̂�������
			if (!obj->GetComponent<Collision>() ||
				!o->GetComponent<Collision>()) {
				continue;
			}
			//�����I�u�W�F�N�g�Ȃ珈�����Ȃ�
			if (o == obj) {
				continue;
			}
			//�R���W�����������o
			Collision* col1 = obj->GetComponent<Collision>();
			Collision* col2 = o->GetComponent<Collision>();

			//�����蔻�菈��
			if (Collision::CheckHit(col1, col2)) {
				obj->Hit(o);
			}
		}
	}

	void ZSort() {
		//�J��������̋����̍~���Ń\�[�g
		Camera* c = GetGameobject<Camera>();
		if (!c) {
			return;
		}
		//�J�����̃|�W�V�������x�N�^�[�^�ɕϊ�
		XMVECTOR camP = XMLoadFloat3(&c->GetPosition());

		//�J������Z�x�N�g�����擾
		XMVECTOR camD = c->GetZDirection();
		//���K��
		XMVector3Normalize(camD);

		for (auto& o : _gameobjects[Object]) {
			XMVECTOR pos = XMLoadFloat3(&o->GetPosition());
			XMVECTOR dir = pos - camP;
			o->SetCamDistance(XMVectorGetX(XMVector3Dot(dir, camD)));
		}

		//Z�\�[�g
		_gameobjects[Object].sort([](GameObject* a, GameObject* b) {
			return (a->GetCamDistance() > b->GetCamDistance());
			});
		//�ˉe�A�����蔻��ł��g��
	}
};
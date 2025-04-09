//Scene.cpp
//20106_�c���@�@
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
			//�`�揈��
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
			//�`�揈��
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

void Scene::ZSort() {
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

	for (auto& o : _gameobjects[static_cast<int>(Layer::Object)]) {
		XMVECTOR pos = XMLoadFloat3(&o->GetPosition());
		XMVECTOR dir = pos - camP;
		o->SetCamDistance(XMVectorGetX(XMVector3Dot(dir, camD)));
	}

	//Z�\�[�g
	_gameobjects[static_cast<int>(Layer::Object)].sort([](GameObject* a, GameObject* b) {
		return (a->GetCamDistance() > b->GetCamDistance());
		});
	//�ˉe�A�����蔻��ł��g��
}

//GameObject.h	ゲームオブジェクト
//20106_田中　蓮
//24_05_15
#pragma once

#include "Main.h"
#include "C_Transform.h"
#include <list>

class Scene;
class Component;

class GameObject {
protected:
	std::list<Component*>	_components{};
	XMMATRIX				_prevworld{};
	bool					_ismotionblur = true;	
	bool					_isdestroy = false;
	float					_cameralength{};

public:
	//コンストラクタ
	GameObject(){
		_components.push_back(new Transform(this));
	}
	virtual ~GameObject() {
		for (auto c : _components) {
			if (!c) {
				continue;
			}
			delete c;
		}
	}

	virtual void Init() = 0 {}
	virtual void Uninit() = 0 {}
	virtual void Update() = 0 {}
	virtual void Draw() = 0 {}

	XMFLOAT3& GetPosition() { return GetComponent<Transform>()->_position; }
	XMFLOAT3& GetRotation() { return GetComponent<Transform>()->_rotation; }
	XMFLOAT3& GetScale() { return GetComponent<Transform>()->_scale; }
	XMFLOAT4& GetQuaternion() { return GetComponent<Transform>()->_quaternion; }
	XMFLOAT3& GetForward() { return GetComponent<Transform>()->_forward; }
	XMFLOAT3& GetUp() { return GetComponent<Transform>()->_up; }
	XMFLOAT3& GetRight() { return GetComponent<Transform>()->_right; }
	float& GetCamDistance() { return _cameralength; }

	void SetPosition(const XMFLOAT3& position) { GetComponent<Transform>()->_position = position; }
	void SetRotation(const XMFLOAT3& rotation) { GetComponent<Transform>()->_rotation = rotation; }
	void SetScale(const XMFLOAT3& scale) { GetComponent<Transform>()->_scale = scale; }
	void SetQuaternion(const XMFLOAT4& quaternion) {
		GetComponent<Transform>()->_quaternion = quaternion;
		GetComponent<Transform>()->UseQuaternion();
	}
	void SetDestroy() { _isdestroy = true; }
	void SetCamDistance(const float& camdis) { _cameralength = camdis; }

	virtual void Hit(GameObject* obj){}

	bool Destroy() {
		if (_isdestroy) {
			Uninit();
			delete this;
			return true;
		}
		else {
			return false;
		}
	}

	//オブジェクトが持っているコンポーネントを取得
	template<class T>
	T* GetComponent(){
		for (auto com : _components) {
			T* buff = dynamic_cast<T*>(com);
			if (buff != nullptr)
				return buff;
		}
		return nullptr;
	}
	//オブジェクトが持っているコンポーネントを追加
	template<class T>
	T* AddComponent(GameObject* parent){
		T* buff = new T(this);
		_components.push_back(buff);
		return buff;
	}
};
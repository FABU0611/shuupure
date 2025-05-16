//C_Collison.h
//20106_田中　蓮
//24_06_05
#pragma once

#include "Main.h"
#include "Component.h"

enum class ColiderType {
	BOX,
	SQUARE,
	CYLINDER,
	CAPSEL,

	TYPE_MAX,
};

class Collision : public Component {
public:
	using Component::Component;

	XMFLOAT3	_position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3	_rotation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3	_scale = { 1.0f, 1.0f, 1.0f };
	float		_radius = 1.0f;

	ColiderType	_type = ColiderType::BOX;

	void SetPosition(const XMFLOAT3& pos) { _position = pos; }
	void SetRotation(const XMFLOAT3& rot) { _rotation = rot; }
	void SetScale(const XMFLOAT3& scl) { _scale = scl; }
	void SetRadius(const float& rad) { _radius = rad; }
	const void SetType(const ColiderType& type) { _type = type; }

	ColiderType GetType()const { return _type; }

	static bool CheckHit(Collision* me, Collision* you) {
		if (me == nullptr || you == nullptr) {
			return false;
		}
		//コリジョンのタイプで分岐
		if (me->_type == ColiderType::BOX && you->_type == ColiderType::BOX) {
			return CheckHitAABB(me, you);
		}
		if (me->_type == ColiderType::SQUARE && you->_type == ColiderType::SQUARE) {
			return CheckHitSphere(me, you);
		}
		return false;
	}

	//箱同士の当たり判定
	static bool CheckHitAABB(Collision* me, Collision* you) {
		//meの右側にyouの左側が当たってか判定
		bool isRight	= me->_position.x + (me->_scale.x * 0.5f) > you->_position.x - (you->_scale.x * 0.5f);

		// 一方の四角形が他方の四角形の右側にあるか判定
		bool isLeft		= me->_position.x - (me->_scale.x * 0.5f) < you->_position.x + (you->_scale.x * 0.5f);

		// 一方の四角形が他方の四角形の上側にあるか判定
		bool isTop		= me->_position.y + (me->_scale.y * 0.5f) > you->_position.y - (you->_scale.y * 0.5f);

		// 一方の四角形が他方の四角形の下側にあるか判定
		bool isBottom	= me->_position.y - (me->_scale.y * 0.5f) < you->_position.y + (you->_scale.y * 0.5f);

		// 一方の四角形が他方の四角形の上側にあるか判定
		bool isFront	= me->_position.z + (me->_scale.z * 0.5f) > you->_position.z - (you->_scale.z * 0.5f);

		// 一方の四角形が他方の四角形の下側にあるか判定
		bool isBack		= me->_position.z - (me->_scale.z * 0.5f) < you->_position.z + (you->_scale.z * 0.5f);

		// 上記の条件がすべて成り立っていれば、当たっている
		return (isRight && isLeft && isTop && isBottom && isFront && isBack);
	}

	//球同士の当たり判定
	static bool CheckHitSphere(Collision* me, Collision* you) {
		return Distance(me->_position, you->_position) < (me->_radius + you->_radius);
	}
};
//PlayerState_rotate.h
//20106_田中　蓮
//24_08_28
#include "PlayerState_rorate.h"
#include "Player.h"
#include "PlayerState_underwater.h"
#include "Manager.h"
#include "Scene.h"
#include "ParticleSpraywarter.h"
#include "ParticleFireup.h"
#include "Time.h"
#include "Input.h"
#include "Score.h"
#include "DataManager.h"

void PlayerStateRotate::Update(){
	//回転前のクォータニオン
	XMVECTOR currentQuat = XMLoadFloat4(&GetPlayer()->GetQuaternion());
	//空間上のZ軸
	XMVECTOR worldZ = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	//回転---------------------------------------------------------------------
	//キーボードでの操作
	if (Input::GetKeyPress('W') && !_isNegaRotateY) {
		GetPlayer()->GetRotation().x += 0.2f;	//回転数カウント用に加算
		_angulvel.x = 0.2f;
		_isPosiRotateY = true;					//逆回転ができないように
	}
	else if (Input::GetKeyPress('S') && !_isPosiRotateY) {
		GetPlayer()->GetRotation().x += -0.2f;
		_angulvel.x = -0.2f;
		_isNegaRotateY = true;

	}
	if (Input::GetKeyPress('A') && !_isPosiRotateX) {
		GetPlayer()->GetRotation().y += -0.2f;
		_angulvel.y = -0.2f;
		_isNegaRotateX = true;
	}
	else if (Input::GetKeyPress('D') && !_isNegaRotateX) {
		GetPlayer()->GetRotation().y += 0.2f;
		_angulvel.y = 0.2f;
		_isPosiRotateX = true;
	}

	//コントローラでの操作
	if (Input::GetLeftStickX(0) > STICK_DEADZONE) {
		GetPlayer()->GetRotation().y += Input::GetLeftStickY(0);
	}
	else if (Input::GetLeftStickX(0) < -STICK_DEADZONE) {
		GetPlayer()->GetRotation().y += Input::GetLeftStickY(0);
	}
	if (Input::GetLeftStickY(0) > STICK_DEADZONE) {
		GetPlayer()->GetRotation().x += Input::GetLeftStickX(0);
	}
	else if (Input::GetLeftStickY(0) < -STICK_DEADZONE) {
		GetPlayer()->GetRotation().x += Input::GetLeftStickX(0);
	}

	//減衰
	_angulvel *= _friction;

	//縦回転
	if (std::abs(_angulvel.x) > 0.001f) {
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(worldZ, _angulvel.x);

		// 現在の回転クォータニオンに新しい回転を掛け合わせる
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//計算後のクォータニオンを入れる
		XMStoreFloat4(&GetPlayer()->GetQuaternion(), currentQuat);
		//プレイヤーのUpを更新
		GetPlayer()->GetComponent<Transform>()->Update();
	}
	//捻り回転
	if (std::abs(_angulvel.y) > 0.001f) {
		//縦回転後のクォータニオンを再計算
		currentQuat = XMLoadFloat4(&GetPlayer()->GetQuaternion());
		
		//ワールドY軸（上方向）を基準にした回転クォータニオンを生成
		XMVECTOR yAxis = XMLoadFloat3(&GetPlayer()->GetUp());
		//クォータニオン回転
		XMVECTOR rotationQuat = XMQuaternionRotationAxis(yAxis, _angulvel.y);

		// 現在の回転クォータニオンに新しい回転を掛け合わせる
		currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);

		//計算後のクォータニオンを入れる
		XMStoreFloat4(&GetPlayer()->GetQuaternion(), currentQuat);
		//プレイヤーのUpを更新
		GetPlayer()->GetComponent<Transform>()->Update();
	}


	// 1回転超えたらカウントする
	if (GetPlayer()->GetRotation().y >= XM_2PI) {
		_twistcount++;
		GetPlayer()->GetRotation().y -= XM_2PI;
	}
	else if (GetPlayer()->GetRotation().y <= -XM_2PI) {
		_twistcount++;
		GetPlayer()->GetRotation().y += XM_2PI;
	}

	if (GetPlayer()->GetRotation().x >= XM_2PI) {
		_rotcount++;
		GetPlayer()->GetRotation().x -= XM_2PI;
	}
	else if (GetPlayer()->GetRotation().x <= -XM_2PI) {
		_rotcount++;
		GetPlayer()->GetRotation().x += XM_2PI;
	}

	//落下処理
	GetPlayer()->Fall(Time::GetDeltaTime());
	
	//着水したら
	if (GetPlayer()->GetPosition().y < 0.0f) {
		//水面との内積
		_dot = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&GetPlayer()->GetUp()), { 0.0f, 1.0f, 0.0f }));
		//絶対値にする
		_dot = fabs(_dot);

		//ファイルに書き込む
		DataManager::SaveData(_twistcount, _rotcount, _dot);

		Scene* scene;
		scene = Manager::GetScene();

		//内積からエフェクトの大きさを決定
		float spray = 2.0f - _dot;
		//エフェクトを出す
		scene->AddGameobject<SprayWater>(Effect, GetPlayer()->GetPosition(), 0.7f, spray);

		if (_dot == 1.0f) {
			GetPlayer()->PlayAudioSml();
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(20.0f, 0.0f, 40.0f), 2.0f, 0.3f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 50.0f);
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(80.0f, 0.0f, 0.0f), 1.5f, 0.4f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 40.0f);
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(20.0f, 0.0f, -40.0f), 1.0f, 0.35f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 30.0f);
		}
		else if (_dot == 0.0f) {
			GetPlayer()->PlayAudioBig();
		}
		else {
			GetPlayer()->PlayAudioMid();
			scene->AddGameobject<Fireup>(Effect, XMFLOAT3(20.0f, 0.0f, 40.0f), 2.0f, 0.3f, XMFLOAT4(0.2f, 0.5f, 0.2f, 1.0f), 50.0f);
		}

		//重力を小さく
		GetPlayer()->GetAccel().y *= 0.5f;

		GetPlayer()->ChangeState(new PlayerStateUnderwater(GetPlayer()));
	}
}

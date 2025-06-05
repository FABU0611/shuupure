//CameraState_Title2.cpp
//20106_“c’†@˜@
//24_12_12
#include "CameraState_Title2.h"
#include "CameraState_Title1.h"
#include "Camera.h"
#include "Time.h"
#include "Manager.h"
#include "Gaussian.h"
#include "PostEffectManager.h"


void TitleState2::Update() {
	Camera* camera = GetCamera();
	_time += Time::GetDeltaTime();

	camera->GetRotation().y -= 0.1f * Time::GetDeltaTime();
	camera->GetRotation().x -= 0.01f * Time::GetDeltaTime();

	if (_time >= 7.0f) {
		PostEffectManager::GetInstance()->GetGaussian()->SetDof({ 0.07f, 0.05f });
		PostEffectManager::GetInstance()->GetGaussian()->SetBoke(10.0f);
		_time = 0.0f;
		camera->SetPosition({ 0.0f, 25.0f, 30.0f });
		camera->SetRotation({ -0.76f, -0.8f, 0.6f });
		camera->SetTarget({ 35.0f, 6.3f, 15.0f });
		float vx, vz;
		vx = camera->GetPosition().x - camera->GetTartgetPos().x;
		vz = camera->GetPosition().z - camera->GetTartgetPos().z;
		float length = sqrtf(vx * vx + vz * vz);
		camera->SetLength(length);
		camera->ChangeState(new TitleState1(camera));
	}
}

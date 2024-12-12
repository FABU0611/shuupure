//CameraState_Title1.cpp
//20106_“c’†@˜@
//24_12_12
#include "CameraState_Title1.h"
#include "CameraState_Title2.h"
#include "Camera.h"
#include "Time.h"
#include "Manager.h"
#include "Gaussian.h"

void TitleState1::Update() {
	Camera* camera = GetCamera();
	_time += Time::GetDeltaTime();

	camera->GetRotation().y -= 0.04f * Time::GetDeltaTime();

	if (_time >= 5.0f) {
		Manager::GetGaussian()->SetDof({ 0.0f, 0.0f });
		Manager::GetGaussian()->SetBoke(0.1f);
		_time = 0.0f;
		camera->SetRotation({ XM_PIDIV4 * 0.8f, XM_PI * 0.8f, 0.0f });
		camera->SetTarget({ 0.0f,0.0f, 0.0f });
		camera->SetLength(50.0f);
		camera->ChangeState(new TitleState2(camera));
	}
}

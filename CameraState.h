//CameraState.h
//20106_�c���@�@
//24_12_12
#pragma once

class Camera;

class CameraState {
protected:
	Camera*		_camera{};
	float		_time{};

	Camera* GetCamera() { return _camera; }

public:
	CameraState(Camera* camera):_camera(camera) {}
	virtual ~CameraState() {}

	virtual void Update() = 0;
};
#pragma once
#include <Xinput.h>

#define MAX_CONTROLLERS 4 // �ő�R���g���[����

static enum MOUSE_BUTTON {
	MOUSE_LBUTTON = 0,	//���N���b�N
	MOUSE_RBUTTON,		//�E�N���b�N
	MOUSE_MBUTTON,		//�z�C�[���{�^��
};

struct CONTROLER_STATE{
	XINPUT_STATE state;
	XINPUT_STATE oldstate;
	XINPUT_VIBRATION vibration;
	bool bConnected;
};

class Input{
	static BYTE		_OldKeyState[256];
	static BYTE		_KeyState[256];

	static BYTE		_OldMouseState[3];
	static BYTE		_MouseState[3];
	static POINT	_OldMousePos;
	static POINT	_MousePos;

	static int		_MouseWheelDelta;

	static CONTROLER_STATE _Gamepad[MAX_CONTROLLERS];

public:
	static void Init();
	static void Uninit();
	static void Update();
	static HRESULT UpdateControllerState();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);
	static bool GetKeyRelease(BYTE KeyCode);

	static bool GetMousePress(int Button);
	static bool GetMouseTrigger(int Button);
	static bool GetMouseRelease(int Button);
	static POINT GetMousePosition();
	static int GetMouseWheelDelta();

	static void OnMouseWheel(int delta);

	static bool GetButtonPress(const int& padindex, WORD Button);
	static bool GetButtonTrigger(const int& padindex, WORD Button);
	static bool GetButtonRelease(const int& padindex, WORD Button);
	static float GetLeftStickX(const int& padindex);	//0.0�`1.0
	static float GetLeftStickY(const int& padindex);	//0.0�`1.0
	static float GetRightStickX(const int& padindex);	//0.0�`1.0
	static float GetRightStickY(const int& padindex);	//0.0�`1.0
	static float GetLeftTrigger(const int& padindex);	//0.0�`1.0
	static float GetRightTrigger(const int& padindex);	//0.0�`1.0
	static void SetLeftVibration(const int& padindex, float speed);
	static void SetRightVibration(const int& padindex, float speed);
	static void StopVibration(const int& padindex);
};
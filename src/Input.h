//Input.h
//20106_田中　蓮
//24_07_23
#pragma once
#include <Xinput.h>

#define MAX_CONTROLLERS (4) // 最大コントローラ数

enum class MOUSE_BUTTON {
	MOUSE_RBUTTON = 0,	//左クリック
	MOUSE_LBUTTON,		//右クリック
	MOUSE_MBUTTON,		//ホイールボタン
};

struct CONTROLER_STATE {
	XINPUT_STATE		State;
	XINPUT_STATE		OldState;
	XINPUT_VIBRATION	Vibration;
	bool				isConnected;
};

class Input {
	static HWND				_hwnd; // ウィンドウハンドルを保持

	static BYTE				_oldkeystate[256];
	static BYTE				_keystate[256];

	static BYTE				_oldmousestate[3];
	static BYTE				_mousestate[3];
	static POINT			_oldmousepos;
	static POINT			_mousepos;

	static int				_oldmousewheeldelta;
	static int				_mousewheeldelta;

	static CONTROLER_STATE	_gamepad[MAX_CONTROLLERS];

public:
	static void Init();
	static void Uninit();
	static void Update();
	static HRESULT UpdateControllerState();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);
	static bool GetKeyRelease(BYTE KeyCode);

	static bool GetMousePress(MOUSE_BUTTON Button);
	static bool GetMouseTrigger(MOUSE_BUTTON Button);
	static bool GetMouseRelease(MOUSE_BUTTON Button);
	static POINT GetMousePosition();
	static int GetMouseWheelDelta();

	static void OnMouseWheel(int delta);

	static bool GetButtonPress(const int& padindex, WORD Button);
	static bool GetButtonTrigger(const int& padindex, WORD Button);
	static bool GetButtonRelease(const int& padindex, WORD Button);
	static float GetLeftStickX(const int& padindex);	//0.0～1.0
	static float GetLeftStickY(const int& padindex);	//0.0～1.0
	static float GetRightStickX(const int& padindex);	//0.0～1.0
	static float GetRightStickY(const int& padindex);	//0.0～1.0
	static float GetLeftTrigger(const int& padindex);	//0.0～1.0
	static float GetRightTrigger(const int& padindex);	//0.0～1.0
	static void SetLeftVibration(const int& padindex, float speed);
	static void SetRightVibration(const int& padindex, float speed);
	static void StopVibration(const int& padindex);

	static void SetWindowHandle(const HWND& hwnd) { _hwnd = hwnd; }
};

#include "Main.h"
#include "Input.h"


BYTE Input::_OldKeyState[256];
BYTE Input::_KeyState[256];
POINT Input::_OldMousePos;
POINT Input::_MousePos;
BYTE Input::_OldMouseState[3];
BYTE Input::_MouseState[3];
int Input::_MouseWheelDelta = 0;
CONTROLER_STATE Input::_Gamepad[MAX_CONTROLLERS];


void Input::Init(){
	memset(_OldKeyState, 0, 256);
	memset(_KeyState, 0, 256);

	ZeroMemory(&_OldMouseState, 3);
	ZeroMemory(&_MouseState, 3);

	GetCursorPos(&_MousePos);
	_MousePos = _OldMousePos;

	_MouseWheelDelta = 0;

	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		ZeroMemory(&_Gamepad[i], sizeof(CONTROLER_STATE));
	}
}

void Input::Uninit()
{


}

HRESULT Input::UpdateControllerState()
{
	DWORD dwResult;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		XInputSetState(i, &_Gamepad[i].vibration);
		_Gamepad[i].oldstate = _Gamepad[i].state;
		dwResult = XInputGetState(i, &_Gamepad[i].state);

		if (dwResult == ERROR_SUCCESS)
			_Gamepad[i].bConnected = true;
		else
			_Gamepad[i].bConnected = false;
	}

	return S_OK;
}

void Input::Update(){
	memcpy(_OldKeyState, _KeyState, 256);
	GetKeyboardState(_KeyState);

	memcpy(_OldMouseState, _MouseState, 3);
	_MouseState[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	_MouseState[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	_MouseState[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	_OldMousePos = _MousePos;
	GetCursorPos(&_MousePos);

	UpdateControllerState();
}

//キーボード入力---------------------------------------------------------------
bool Input::GetKeyPress(BYTE KeyCode){
	return (_KeyState[KeyCode] & 0x80);
}
bool Input::GetKeyTrigger(BYTE KeyCode){
	return ((_KeyState[KeyCode] & 0x80) && !(_OldKeyState[KeyCode] & 0x80));
}
bool Input::GetKeyRelease(BYTE KeyCode){
	return (!(_KeyState[KeyCode] & 0x80) && (_OldKeyState[KeyCode] & 0x80));
}

//マウス入力-------------------------------------------------------------------
bool Input::GetMousePress(int Button){
	return _MouseState[Button];
}
bool Input::GetMouseTrigger(int Button){
	return (_MouseState[Button] && !_OldMouseState[Button]);
}
bool Input::GetMouseRelease(int Button){
	return (!_MouseState[Button] && _OldMouseState[Button]);
}
POINT Input::GetMousePosition(){
	return _MousePos;
}
int Input::GetMouseWheelDelta(){
	return _MouseWheelDelta;
}
void Input::OnMouseWheel(int delta) {
	_MouseWheelDelta = delta;
}

//コントローラ入力-------------------------------------------------------------
bool Input::GetButtonPress(const int& padindex, WORD Button) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return (_Gamepad[padindex].state.Gamepad.wButtons & Button) != 0;
	}
}
bool Input::GetButtonTrigger(const int& padindex, WORD Button) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return ((_Gamepad[padindex].state.Gamepad.wButtons & Button) != 0) &&
			((_Gamepad[padindex].oldstate.Gamepad.wButtons & Button) == 0);
	}
}
bool Input::GetButtonRelease(const int& padindex, WORD Button) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return ((_Gamepad[padindex].state.Gamepad.wButtons & Button) == 0) &&
			((_Gamepad[padindex].oldstate.Gamepad.wButtons & Button) != 0);
	}
}
//スティック
float Input::GetLeftStickX(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _Gamepad[padindex].state.Gamepad.sThumbLX / 32767.0f;
	}
}
float Input::GetLeftStickY(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _Gamepad[padindex].state.Gamepad.sThumbLY / 32767.0f;
	}
}
float Input::GetRightStickX(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _Gamepad[padindex].state.Gamepad.sThumbRX / 32767.0f;
	}
}
float Input::GetRightStickY(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _Gamepad[padindex].state.Gamepad.sThumbRY / 32767.0f;
	}
}
//トリガー
float Input::GetLeftTrigger(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _Gamepad[padindex].state.Gamepad.bLeftTrigger / 255.0f;
	}
}
float Input::GetRightTrigger(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _Gamepad[padindex].state.Gamepad.bRightTrigger / 255.0f;
	}
}
//バイブレーション
void Input::SetLeftVibration(const int& padindex, float speed) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		_Gamepad[padindex].vibration.wLeftMotorSpeed = (speed * 65535.0f);
	}
}
void Input::SetRightVibration(const int& padindex, float speed) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		_Gamepad[padindex].vibration.wRightMotorSpeed = (speed * 65535.0f);
	}
}
void Input::StopVibration(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		SetLeftVibration(padindex, 0.0f);
		SetRightVibration(padindex, 0.0f);
	}
}
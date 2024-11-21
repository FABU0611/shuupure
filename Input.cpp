
#include "Main.h"
#include "Input.h"


HWND Input::_hwnd;
BYTE Input::_oldkeystate[256];
BYTE Input::_keystate[256];
POINT Input::_oldmousepos;
POINT Input::_mousepos;
BYTE Input::_oldmousestate[3];
BYTE Input::_mousestate[3];
int Input::_mousewheeldelta = 0;
CONTROLER_STATE Input::_gamepad[MAX_CONTROLLERS];


void Input::Init(){
	memset(_oldkeystate, 0, 256);
	memset(_keystate, 0, 256);

	ZeroMemory(&_oldmousestate, 3);
	ZeroMemory(&_mousestate, 3);

	GetCursorPos(&_mousepos);
	_mousepos = _oldmousepos;

	_mousewheeldelta = 0;

	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		ZeroMemory(&_gamepad[i], sizeof(CONTROLER_STATE));
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
		XInputSetState(i, &_gamepad[i].vibration);
		_gamepad[i].oldstate = _gamepad[i].state;
		dwResult = XInputGetState(i, &_gamepad[i].state);

		if (dwResult == ERROR_SUCCESS)
			_gamepad[i].bConnected = true;
		else
			_gamepad[i].bConnected = false;
	}

	return S_OK;
}

void Input::Update(){
	memcpy(_oldkeystate, _keystate, 256);
	GetKeyboardState(_keystate);

	memcpy(_oldmousestate, _mousestate, 3);
	_mousestate[0] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	_mousestate[1] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	_mousestate[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	_oldmousepos = _mousepos;
	//現在のマウス位置を取得（画面全体の座標）
	GetCursorPos(&_mousepos);
	//マウス位置をクライアント座標に変換（ウィンドウ内の座標系に変更）
	ScreenToClient(_hwnd, &_mousepos);

	UpdateControllerState();
}

//キーボード入力---------------------------------------------------------------
bool Input::GetKeyPress(BYTE KeyCode){
	return (_keystate[KeyCode] & 0x80);
}
bool Input::GetKeyTrigger(BYTE KeyCode){
	return ((_keystate[KeyCode] & 0x80) && !(_oldkeystate[KeyCode] & 0x80));
}
bool Input::GetKeyRelease(BYTE KeyCode){
	return (!(_keystate[KeyCode] & 0x80) && (_oldkeystate[KeyCode] & 0x80));
}

//マウス入力-------------------------------------------------------------------
bool Input::GetMousePress(int Button){
	return _mousestate[Button];
}
bool Input::GetMouseTrigger(int Button){
	return (_mousestate[Button] && !_oldmousestate[Button]);
}
bool Input::GetMouseRelease(int Button){
	return (!_mousestate[Button] && _oldmousestate[Button]);
}
POINT Input::GetMousePosition(){
	return _mousepos;
}
int Input::GetMouseWheelDelta(){
	return _mousewheeldelta;
}
void Input::OnMouseWheel(int delta) {
	_mousewheeldelta = delta;
}

//コントローラ入力-------------------------------------------------------------
bool Input::GetButtonPress(const int& padindex, WORD Button) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return (_gamepad[padindex].state.Gamepad.wButtons & Button) != 0;
	}
}
bool Input::GetButtonTrigger(const int& padindex, WORD Button) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return ((_gamepad[padindex].state.Gamepad.wButtons & Button) != 0) &&
			((_gamepad[padindex].oldstate.Gamepad.wButtons & Button) == 0);
	}
}
bool Input::GetButtonRelease(const int& padindex, WORD Button) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return ((_gamepad[padindex].state.Gamepad.wButtons & Button) == 0) &&
			((_gamepad[padindex].oldstate.Gamepad.wButtons & Button) != 0);
	}
}
//スティック
float Input::GetLeftStickX(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _gamepad[padindex].state.Gamepad.sThumbLX / 32767.0f;
	}
}
float Input::GetLeftStickY(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _gamepad[padindex].state.Gamepad.sThumbLY / 32767.0f;
	}
}
float Input::GetRightStickX(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _gamepad[padindex].state.Gamepad.sThumbRX / 32767.0f;
	}
}
float Input::GetRightStickY(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _gamepad[padindex].state.Gamepad.sThumbRY / 32767.0f;
	}
}
//トリガー
float Input::GetLeftTrigger(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _gamepad[padindex].state.Gamepad.bLeftTrigger / 255.0f;
	}
}
float Input::GetRightTrigger(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		return _gamepad[padindex].state.Gamepad.bRightTrigger / 255.0f;
	}
}
//バイブレーション
void Input::SetLeftVibration(const int& padindex, float speed) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		_gamepad[padindex].vibration.wLeftMotorSpeed = (speed * 65535.0f);
	}
}
void Input::SetRightVibration(const int& padindex, float speed) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		_gamepad[padindex].vibration.wRightMotorSpeed = (speed * 65535.0f);
	}
}
void Input::StopVibration(const int& padindex) {
	if (padindex >= 0 && padindex < MAX_CONTROLLERS) {
		SetLeftVibration(padindex, 0.0f);
		SetRightVibration(padindex, 0.0f);
	}
}
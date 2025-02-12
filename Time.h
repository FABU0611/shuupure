//Time.h
//20106_田中　蓮
//24_08_28
#pragma once
#include <Windows.h>
#include <string>

class Time{
    static LARGE_INTEGER    _frequency;  //タイマーの周波数
    static LARGE_INTEGER    _lastTime;   //前回のフレーム時間
	static float            _deltatime;  //フレーム間の経過時間
	static float            _gamespeed;  //ゲームスピード

public:
    static void Init();
    static void Update();

    //フレーム間の経過時間を取得
    static const float& GetDeltaTime();
    //現在のフレームレートを取得
    static const float& GetFrameRate();
	static const float& GetGameSpeed() { return _gamespeed; }
    static const std::wstring GetNowRealTime();

	static void SetGameSpeed(const float& speed) { _gamespeed = speed; }

    static void GamePause();
	static void GameResume();
};
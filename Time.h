//Time.h
//20106_田中　蓮
//24_08_28
#pragma once
#include<Windows.h>

class Time{
    static LARGE_INTEGER    _frequency;  //タイマーの周波数
    static LARGE_INTEGER    _lastTime;   //前回のフレーム時間
    static float            _deltatime;

public:
    static void Init();
    static void Update();

    //フレーム間の経過時間を取得
    static float GetDeltaTime();

    //現在のフレームレートを取得
    static float GetFrameRate();
};
//Time.cpp
//20106_田中　蓮
//24_08_28
#include "Time.h"

LARGE_INTEGER Time::_frequency;
LARGE_INTEGER Time::_lastTime;
float Time::_deltatime;

void Time::Init() {
    QueryPerformanceFrequency(&_frequency); // タイマーの周波数を取得
    QueryPerformanceCounter(&_lastTime);    // 最初のタイムスタンプを取得
}

void Time::Update(){
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);  // 現在のタイムスタンプを取得
    _deltatime = static_cast<float>(currentTime.QuadPart - _lastTime.QuadPart) / _frequency.QuadPart;
    _lastTime = currentTime; // 次のフレームのために時間を更新
}

float Time::GetDeltaTime() {
    return _deltatime;
}

float Time::GetFrameRate() {
    float deltaTime = GetDeltaTime();
    return 1.0f / deltaTime;
}

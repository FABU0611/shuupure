//Time.cpp
//20106_田中　蓮
//24_08_28
#include "Time.h"
#include <chrono>
#include <sstream>

LARGE_INTEGER Time::_frequency;
LARGE_INTEGER Time::_lastTime;
float Time::_deltatime;
float Time::_gamespeed;

void Time::Init() {
    QueryPerformanceFrequency(&_frequency); //タイマーの周波数を取得
    QueryPerformanceCounter(&_lastTime);    //最初のタイムスタンプを取得
	_gamespeed = 1.0f;
}

void Time::Update(){
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);  //現在のタイムスタンプを取得
    _deltatime = static_cast<float>(currentTime.QuadPart - _lastTime.QuadPart) / _frequency.QuadPart;
    _lastTime = currentTime;                //次のフレームのために時間を更新
}

const float Time::GetDeltaTime() {
    return _deltatime * _gamespeed;
}

const float Time::GetFrameRate() {
    float deltaTime = GetDeltaTime();
    return 1.0f / deltaTime;
}

const std::wstring Time::GetNowRealTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    //ローカル時間に変換
    std::tm localTime;
    localtime_s(&localTime, &time);

    //フォーマットされた日時文字列を作成
    std::wstringstream ss;
    ss << (1900 + localTime.tm_year) << L"_"
        << std::setw(2) << std::setfill(L'0') << (1 + localTime.tm_mon) << L"_"
        << std::setw(2) << std::setfill(L'0') << localTime.tm_mday << L"_"
        << std::setw(2) << std::setfill(L'0') << localTime.tm_hour << L"_"
        << std::setw(2) << std::setfill(L'0') << localTime.tm_min << L"_"
        << std::setw(2) << std::setfill(L'0') << localTime.tm_sec;

    return ss.str();
}

void Time::GamePause() {
	_deltatime = 0.0f;
}

void Time::GameResume() {
    QueryPerformanceCounter(&_lastTime);    //最初のタイムスタンプを取得
}

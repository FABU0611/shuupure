//Time.cpp
//20106_�c���@�@
//24_08_28
#include "Time.h"
#include <chrono>
#include <sstream>

LARGE_INTEGER Time::_frequency;
LARGE_INTEGER Time::_lastTime;
float Time::_deltatime;
float Time::_gamespeed;

void Time::Init() {
    QueryPerformanceFrequency(&_frequency); //�^�C�}�[�̎��g�����擾
    QueryPerformanceCounter(&_lastTime);    //�ŏ��̃^�C���X�^���v���擾
	_gamespeed = 1.0f;
}

void Time::Update(){
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);  //���݂̃^�C���X�^���v���擾
    _deltatime = static_cast<float>(currentTime.QuadPart - _lastTime.QuadPart) / _frequency.QuadPart;
    _lastTime = currentTime;                //���̃t���[���̂��߂Ɏ��Ԃ��X�V
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

    //���[�J�����Ԃɕϊ�
    std::tm localTime;
    localtime_s(&localTime, &time);

    //�t�H�[�}�b�g���ꂽ������������쐬
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
    QueryPerformanceCounter(&_lastTime);    //�ŏ��̃^�C���X�^���v���擾
}

//Time.cpp
//20106_�c���@�@
//24_08_28
#include "Time.h"

LARGE_INTEGER Time::_frequency;
LARGE_INTEGER Time::_lastTime;
float Time::_deltatime;

void Time::Init() {
    QueryPerformanceFrequency(&_frequency); // �^�C�}�[�̎��g�����擾
    QueryPerformanceCounter(&_lastTime);    // �ŏ��̃^�C���X�^���v���擾
}

void Time::Update(){
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);  // ���݂̃^�C���X�^���v���擾
    _deltatime = static_cast<float>(currentTime.QuadPart - _lastTime.QuadPart) / _frequency.QuadPart;
    _lastTime = currentTime; // ���̃t���[���̂��߂Ɏ��Ԃ��X�V
}

float Time::GetDeltaTime() {
    return _deltatime;
}

float Time::GetFrameRate() {
    float deltaTime = GetDeltaTime();
    return 1.0f / deltaTime;
}

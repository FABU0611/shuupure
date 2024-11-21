//Time.h
//20106_�c���@�@
//24_08_28
#pragma once
#include<Windows.h>

class Time{
    static LARGE_INTEGER    _frequency;  //�^�C�}�[�̎��g��
    static LARGE_INTEGER    _lastTime;   //�O��̃t���[������
    static float            _deltatime;

public:
    static void Init();
    static void Update();

    //�t���[���Ԃ̌o�ߎ��Ԃ��擾
    static float GetDeltaTime();

    //���݂̃t���[�����[�g���擾
    static float GetFrameRate();
};
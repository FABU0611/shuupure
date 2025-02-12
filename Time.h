//Time.h
//20106_�c���@�@
//24_08_28
#pragma once
#include <Windows.h>
#include <string>

class Time{
    static LARGE_INTEGER    _frequency;  //�^�C�}�[�̎��g��
    static LARGE_INTEGER    _lastTime;   //�O��̃t���[������
	static float            _deltatime;  //�t���[���Ԃ̌o�ߎ���
	static float            _gamespeed;  //�Q�[���X�s�[�h

public:
    static void Init();
    static void Update();

    //�t���[���Ԃ̌o�ߎ��Ԃ��擾
    static const float& GetDeltaTime();
    //���݂̃t���[�����[�g���擾
    static const float& GetFrameRate();
	static const float& GetGameSpeed() { return _gamespeed; }
    static const std::wstring GetNowRealTime();

	static void SetGameSpeed(const float& speed) { _gamespeed = speed; }

    static void GamePause();
	static void GameResume();
};
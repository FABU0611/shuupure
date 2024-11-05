#pragma once

#include <xaudio2.h>
#include "gameObject.h"
#include "Component.h"
#include <string>
#include <unordered_map>

struct Sound{
	IXAudio2SourceVoice* _SourceVoice{};
	BYTE* _SoundData{};

	int						_Length{};
	int						_PlayLength{};
};

class Audio : public Component{
	static std::unordered_map<std::string, Sound*> _AudioPool;

	static IXAudio2*				_Xaudio;
	static IXAudio2MasteringVoice*	_MasteringVoice;

	Sound* _sound{};

public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	static void UninitAll();

	void Load(const char *FileName);
	void Play(const char* FileName, bool Loop = false);

	void StopAudio(const char* FileName);
	void SetVolume(const char* FileName, const float& volume);
	bool IsPlaying(const char* FileName);		//çƒê∂íÜÇ©ï‘Ç∑
	void FadeOut(const char* FileName, const float& rate);
	void FadeIn(const char* FileName, const float& rate);
};



#include "main.h"
#include "C_Audio.h"





IXAudio2*				Audio::_Xaudio = NULL;
IXAudio2MasteringVoice*	Audio::_MasteringVoice = NULL;
std::unordered_map<std::string, Sound*> Audio::_AudioPool;


void Audio::InitMaster(){
	// COM������
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio����
	XAudio2Create(&_Xaudio, 0);

	// �}�X�^�����O�{�C�X����
	_Xaudio->CreateMasteringVoice(&_MasteringVoice);
}


void Audio::UninitMaster(){
	_MasteringVoice->DestroyVoice();
	_Xaudio->Release();
	CoUninitialize();
}









void Audio::Load(const char *FileName){

	// �T�E���h�f�[�^�Ǎ�
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		//�ǂݍ��܂�Ă���T�E���h���𒲂ׂ�
		if (_AudioPool.count(FileName) > 0) {
			_sound = _AudioPool[FileName];
			return;
		}
		//�Ȃ�������V�������
		_sound = new Sound;
		_AudioPool[FileName] = _sound;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX)){
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		_AudioPool[FileName]->_SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)_AudioPool[FileName]->_SoundData, buflen);


		_AudioPool[FileName]->_Length = readlen;
		_AudioPool[FileName]->_PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// �T�E���h�\�[�X����
	_Xaudio->CreateSourceVoice(&_AudioPool[FileName]->_SourceVoice, &wfx);
	assert(_AudioPool[FileName]->_SourceVoice);
}


void Audio::UninitAll(){
	//���ׂĂ�Sound�̏I������������
	for (std::pair<const std::string, Sound*> pair : _AudioPool) {
		pair.second->_SourceVoice->Stop();
		pair.second->_SourceVoice->DestroyVoice();

		delete[] pair.second->_SoundData;

		delete pair.second;
	}
	//�S������
	_AudioPool.clear();
}





void Audio::Play(const char* FileName, bool Loop)
{
	_AudioPool[FileName]->_SourceVoice->Stop();
	_AudioPool[FileName]->_SourceVoice->FlushSourceBuffers();


	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = _AudioPool[FileName]->_Length;
	bufinfo.pAudioData = _AudioPool[FileName]->_SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = _AudioPool[FileName]->_PlayLength;

	// ���[�v�ݒ�
	if (Loop){
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = _AudioPool[FileName]->_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	_AudioPool[FileName]->_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

/*
	float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
	m_SourceVoice->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
	//m_SourceVoice->SetVolume(0.1f);
*/


	// �Đ�
	_AudioPool[FileName]->_SourceVoice->Start();

}

void Audio::StopAudio(const char* FileName){
	_AudioPool[FileName]->_SourceVoice->Stop();
}

void Audio::SetVolume(const char* FileName, const float& volume){
	_AudioPool[FileName]->_SourceVoice->SetVolume(volume);
}

bool Audio::IsPlaying(const char* FileName){
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	_AudioPool[FileName]->_SourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Audio::FadeOut(const char* FileName, const float& rate){
	float volume;
	 _AudioPool[FileName]->_SourceVoice->GetVolume(&volume);
	 if (volume <= 0.0f) {
		 return;
	 }
	 volume -= rate;
	 _AudioPool[FileName]->_SourceVoice->SetVolume(volume);
}

void Audio::FadeIn(const char* FileName, const float& rate){
	float volume;
	_AudioPool[FileName]->_SourceVoice->GetVolume(&volume);
	if (volume >= 1.0f) {
		return;
	}
	volume += rate;
	_AudioPool[FileName]->_SourceVoice->SetVolume(volume);
}




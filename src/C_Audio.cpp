
#include "main.h"
#include "C_Audio.h"





IXAudio2* Audio::_xaudio = NULL;
IXAudio2MasteringVoice* Audio::_masteringvoice = NULL;
std::unordered_map<std::string, Sound*> Audio::_audiopool;


void Audio::InitMaster() {
	// COM初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio生成
	XAudio2Create(&_xaudio, 0);

	// マスタリングボイス生成
	_xaudio->CreateMasteringVoice(&_masteringvoice);
}


void Audio::UninitMaster() {
	_masteringvoice->DestroyVoice();
	_xaudio->Release();
	CoUninitialize();
}









void Audio::Load(const char* FileName) {

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		//読み込まれているサウンド名を調べる
		if (_audiopool.count(FileName) > 0) {
			_sound = _audiopool[FileName];
			return;
		}
		//なかったら新しく作る
		_sound = new Sound;
		_audiopool[FileName] = _sound;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX)) {
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else {
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
		_audiopool[FileName]->SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)_audiopool[FileName]->SoundData, buflen);


		_audiopool[FileName]->Length = readlen;
		_audiopool[FileName]->PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	_xaudio->CreateSourceVoice(&_audiopool[FileName]->SourceVoice, &wfx);
	assert(_audiopool[FileName]->SourceVoice);
}


void Audio::UninitAll() {
	//すべてのSoundの終了処理をする
	for (std::pair<const std::string, Sound*> pair : _audiopool) {
		pair.second->SourceVoice->Stop();
		pair.second->SourceVoice->DestroyVoice();

		delete[] pair.second->SoundData;

		delete pair.second;
	}
	//全部消す
	_audiopool.clear();
}





void Audio::Play(const char* FileName, bool Loop) {
	if (!_audiopool.contains(FileName)) {
		return;
	}

	_audiopool[FileName]->SourceVoice->Stop();
	_audiopool[FileName]->SourceVoice->FlushSourceBuffers();


	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = _audiopool[FileName]->Length;
	bufinfo.pAudioData = _audiopool[FileName]->SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = _audiopool[FileName]->PlayLength;

	// ループ設定
	if (Loop) {
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = _audiopool[FileName]->PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	_audiopool[FileName]->SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	/*
		float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
		m_SourceVoice->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
		//m_SourceVoice->SetVolume(0.1f);
	*/


	// 再生
	_audiopool[FileName]->SourceVoice->Start();

}

void Audio::StopAudio(const char* FileName) {
	if (!_audiopool.contains(FileName)) {
		return;
	}

	_audiopool[FileName]->SourceVoice->Stop();
}

void Audio::SetVolume(const char* FileName, const float& volume) {
	if (!_audiopool.contains(FileName)) {
		return;
	}

	_audiopool[FileName]->SourceVoice->SetVolume(volume);
}

bool Audio::IsPlaying(const char* FileName) {
	if (!_audiopool.contains(FileName)) {
		return false;
	}

	XAUDIO2_VOICE_STATE xa2state;

	//状態取得
	_audiopool[FileName]->SourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

void Audio::FadeOut(const char* FileName, const float& rate) {
	if (!_audiopool.contains(FileName)) {
		return;
	}

	float volume;
	_audiopool[FileName]->SourceVoice->GetVolume(&volume);
	if (volume <= 0.0f) {
		return;
	}
	volume -= rate;
	_audiopool[FileName]->SourceVoice->SetVolume(volume);
}

void Audio::FadeIn(const char* FileName, const float& rate) {
	if (!_audiopool.contains(FileName)) {
		return;
	}

	float volume;
	_audiopool[FileName]->SourceVoice->GetVolume(&volume);
	if (volume >= 1.0f) {
		return;
	}
	volume += rate;
	_audiopool[FileName]->SourceVoice->SetVolume(volume);
}




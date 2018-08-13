#pragma once
#include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <mmsystem.h>
#include "PlaybackThread.h"
#include "WaveOutClass.h"
#pragma comment(lib,"Winmm.lib")
struct FileData;
class PlaybackThread;
class WaveOutClass;
class VoiceNodeThread
{
public:
	enum ThreadStatus
	{
		waiting,
		play,
		playing,
		end
	};
	VoiceNodeThread(XAUDIO2_BUFFER buff, HWAVEOUT waveOut, WaveOutClass* waveoutclass);
	VoiceNodeThread() = delete;
	~VoiceNodeThread();
	void setBuffer(XAUDIO2_BUFFER buff)
	{
		buff;
		//buffer = buff;
		//pVoice->FlushSourceBuffers();
		//pVoice->SubmitSourceBuffer(&buffer);
		//pVoice->Start(0);
	}
	void setBuffer(FileData& data);
	void launchThread();
	void playAudio();
	void endPlay();
	void killThread();
	PlaybackThread* pPlayback = nullptr;
	void VoiceNodeThread::writeAudioBlock();
	bool isDataLoaded()
	{
		return dataLoaded;
	}
	bool isFinalDataNode()
	{
		return finalDataNode;
	}
private:
	WAVEHDR header;
	//XAUDIO2_BUFFER buffer;
	IXAudio2SourceVoice* pVoice;
	ThreadStatus status = waiting;
	//ThreadStatus s = play;
	std::mutex lock;
	std::condition_variable cv;
	HWAVEOUT hWaveOut;
	bool dataLoaded = false;
	bool finalDataNode = false;
	WaveOutClass* pWaveOut;
};


#pragma once
#include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <mmsystem.h>
#include "PlaybackThread.h"
#include "PlaybackThread.h"
#pragma comment(lib,"Winmm.lib")
class PlaybackThread;
class WaveOutClass
{
public:
	WaveOutClass(HWAVEOUT waveOut);
	~WaveOutClass();
	void writeAudioBlock(WAVEHDR& header);
	void setPlayback(PlaybackThread* playback)
	{
		pPlayback = playback;
	}
private:
	HWAVEOUT hWaveOut;
	PlaybackThread* pPlayback;
	std::mutex lock;
};


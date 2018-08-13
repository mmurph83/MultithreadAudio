
#include "AudioEngineCore.h"


class VoiceThread
{
public:
	VoiceThread() = delete;
	VoiceThread(XAUDIO2_BUFFER* buff, IXAudio2SourceVoice* voice);
	~VoiceThread();
	void operator()()
	{
		while (isAlive)
		{
			if (playAudio)
			{
				//pSourceVoice->Start(0);
				playAudio = false;
			}
		}
	}
	void setBuffer(XAUDIO2_BUFFER* buff)
	{
		pSourceVoice->Stop(0);
		pSourceVoice->FlushSourceBuffers();
		buffer = buff;
		pSourceVoice->SubmitSourceBuffer(buffer);
	}
	void startAudio()
	{
		playAudio = true;
	}
	void TerminateThread()
	{
		isAlive = false;
	}
private:
	XAUDIO2_BUFFER* buffer;
	IXAudio2SourceVoice* pSourceVoice;
	bool playAudio = false;
	bool isAlive = true;
	bool isPlaying = false;
};

#include "VoiceNodeThread.h"

static VoiceNodeThread* pThread;
static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, int x)
{
	if (uMsg == WOM_DONE)
	{
		//pThread->endPlay();
	}
	if (uMsg == WOM_OPEN)
	{
		//printf("hi");
	}
}


VoiceNodeThread::VoiceNodeThread(XAUDIO2_BUFFER buff, HWAVEOUT waveOut, WaveOutClass* waveoutclass) :hWaveOut(waveOut), pWaveOut(waveoutclass)
{
	
}


VoiceNodeThread::~VoiceNodeThread()
{
}

void VoiceNodeThread::setBuffer(FileData& data)
{
	unsigned int hdrSize = sizeof(WAVEHDR);
	ZeroMemory(&header, hdrSize);
	header.dwBufferLength = data.size;
	header.lpData = (LPSTR)data.fileData;
	dataLoaded = true;
	finalDataNode = data.finalBuffer;
	cv.notify_one();

}
void VoiceNodeThread::writeAudioBlock()
{

	pThread = this;
	unsigned int hdrSize = sizeof(WAVEHDR);

	pWaveOut->writeAudioBlock(header);
	status = waiting;

}

void VoiceNodeThread::launchThread()
{
	
	while (status != end)
	{
		std::unique_lock<std::mutex> lk(lock);
		cv.wait(lk, [this] {return status != waiting; });
		
		if (status == play)
		{
			writeAudioBlock();
			status = waiting;
		}
	}
}

void VoiceNodeThread::playAudio()
{
	if (status != end)
	{
			status = play;
			cv.notify_one();
	}
}
void VoiceNodeThread::endPlay()
{
	status = waiting;
	dataLoaded = false;
	//pPlayback->notifyAudioCompletion();
}

void VoiceNodeThread::killThread()
{
	status = end;
	cv.notify_one();
}
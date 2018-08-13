#include "PlaybackThread.h"



PlaybackThread::PlaybackThread()
{
}


PlaybackThread::~PlaybackThread()
{
}

void PlaybackThread::addThread(VoiceNodeThread* voice, std::thread& t, unsigned int i)
{
	if (pCurrent == nullptr)
	{
		pCurrent = new VoiceAndThread();
		pCurrent->pVoiceThread = voice;
		pCurrent->thread = std::move(t);
		pCurrent->id = i;
		pCurrent->pNext = nullptr;
		pEmpty = pCurrent;
		pPlaying = pCurrent;
	}
	else
	{
		pEmpty->pNext = new VoiceAndThread();
		pEmpty = pEmpty->pNext;
		pEmpty->pVoiceThread = voice;
		pEmpty->thread = std::move(t);
		pEmpty->id = i;
		pEmpty->pNext = nullptr;
	}

	//threadList[i].pVoiceThread = voice;
	//threadList[i].thread = std::move(t);
}

void PlaybackThread::setData(FileData& data)
{
	while (pPlaying->id == pEmpty->id && pEmpty->pVoiceThread->isDataLoaded());
	pEmpty->pVoiceThread->setBuffer(data);
	pEmpty = pEmpty->pNext;
}

void PlaybackThread::startThread()
{
	lockVoiceNode();
	audioCompleted = true;
	pCurrent->pVoiceThread->playAudio();
	pCurrent = pCurrent->pNext;
	while (streaming)
	{
		if (pCurrent->id != pPlaying->id && pCurrent->pVoiceThread->isDataLoaded())
		{

			audioCompleted = false;
			pCurrent->pVoiceThread->playAudio();
			pCurrent = pCurrent->pNext;
		}
	}
	printf("Exited Loop\n");
	pEmpty = pCurrent;
	pCurrent = pCurrent->pNext;
	pEmpty->pNext = nullptr;
	while (pCurrent != nullptr)
	{
		pCurrent->pVoiceThread->killThread();
		pCurrent->thread.join();
		pCurrent = pCurrent->pNext;
	}
}

void PlaybackThread::notifyAudioCompletion()
{
	audioCompleted = true;
	if (pPlaying != nullptr)
	{
		if (pPlaying->pVoiceThread->isFinalDataNode())
		{
			coordinatorThread->FinalNodeRead();
		}
		pPlaying = pPlaying->pNext;
		if (coordinatorThread != nullptr)
		{
			coordinatorThread->requestDataChunk();
		}
	}
}

void PlaybackThread::setCoordinatorThread(DataCoordinatorThread* coordinator)
{
	coordinatorThread = coordinator;
	coordinatorThread->setPlaybackThread(this);
}


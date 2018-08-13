#pragma once
#include "VoiceNodeThread.h"
#include "DataCoordinatorThread.h"
#include <thread>
class VoiceNodeThread;
class DataCoordinatorThread;
class PlaybackThread
{
public:
	
	PlaybackThread();
	~PlaybackThread();
	void addThread(VoiceNodeThread* voice, std::thread& t, unsigned int i);
	void setData(FileData& data);
	void startThread();
	void notifyAudioCompletion();
	void lockVoiceNode()
	{
		if (!voicesLocked)
		{
			pEmpty->pNext = pCurrent;
			voicesLocked = true;
			pEmpty = pCurrent;
			//pEmpty = pEmpty->pNext;
		}
	}
	void setCoordinatorThread(DataCoordinatorThread* coordinator);
	void setStream(bool stream)
	{
		streaming = stream;
	}
private:
	struct VoiceAndThread
	{
		VoiceNodeThread* pVoiceThread;
		VoiceAndThread* pNext;
		std::thread thread;
		unsigned int id;
	};
	VoiceAndThread *pEmpty = nullptr;
	VoiceAndThread *pCurrent = nullptr;
	VoiceAndThread *pPlaying = nullptr;
	DataCoordinatorThread* coordinatorThread = nullptr;
	unsigned int id;
	bool audioCompleted = false;
	bool fileCompletelyLoaded = false;
	bool voicesLocked = false;
	bool streaming = true;
	int loadedLoop = 0;
	int playLoop = 0;
};


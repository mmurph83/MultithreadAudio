#pragma once
#include "FileReadAndChunk.h"
#include "FileData.h"
#include "PlaybackThread.h"
#include "FileLoadThread.h"
#include <stdio.h>

class PlaybackThread;
class FileLoadThread;
class DataCoordinatorThread
{
public:
	DataCoordinatorThread();
	~DataCoordinatorThread();
	void addFileData(FileData data)
	{
		int size = 2000;
		if (chunkA == nullptr)
		{
			chunkA = new FileReadAndChunk(data.fileData, data.size, size);
		//	printf("new A added\n");
		}
		else if(chunkB == nullptr)
		{
			chunkB = new FileReadAndChunk(data.fileData, data.size, size);
			//printf("new B added\n");
		}
		else if (chunkA->checkStatus() == FileReadAndChunk::depleted)
		{
			FileReadAndChunk* temp = chunkA;
			chunkA = chunkB;
			chunkB->OverwriteChunk(data.fileData, data.size);
			//chunkB = new FileReadAndChunk(data.fileData, data.size, size);
		//	printf("A Depleted\n");
			//delete temp;
		}
		else if (chunkB->checkStatus() == FileReadAndChunk::depleted)
		{
			chunkB->OverwriteChunk(data.fileData, data.size);
			//FileReadAndChunk* temp = chunkB;
			//chunkB = new FileReadAndChunk(data.fileData, data.size, size);
			//delete temp;
			//printf("B Depleted\n");
			//delete temp;
		}
		//printf("data added\n");
	}
	void signalDataLoad()
	{
		dataLoaded = true;
	}
	void FinalNodeRead()
	{
		finalNodeRead = true;
	}
	void launchThread();
	void sendDataChunk();
	void requestDataChunk()
	{
		dataRequests++;
	}
	void setPlaybackThread(PlaybackThread* thread)
	{
		playbackThread = thread;
	}
	FileLoadThread* pFileThread = nullptr;
private:
	FileReadAndChunk* chunkA = nullptr;
	FileReadAndChunk* chunkB = nullptr;
	PlaybackThread* playbackThread = nullptr;
	int dataRequests = 10;
	bool dataLoaded = false;
	bool finalNodeRead = false;
};


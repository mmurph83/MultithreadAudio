#include "DataCoordinatorThread.h"



DataCoordinatorThread::DataCoordinatorThread()
{
}


DataCoordinatorThread::~DataCoordinatorThread()
{
}

void DataCoordinatorThread::launchThread()
{
	playbackThread->setStream(true);
	while (chunkA->checkStatus() != FileReadAndChunk::depleted || chunkB->checkStatus() != FileReadAndChunk::depleted)
	{
			if (chunkA->checkStatus() != FileReadAndChunk::depleted)
			{
				FileData data = chunkA->createChunk();
				if (playbackThread != nullptr)
				{
					playbackThread->setData(data);
				}
			}
			else
			{
				FileReadAndChunk* temp = chunkA;
				chunkA = chunkB;
				chunkB = temp;
				pFileThread->CreateData();
			}

			if (dataLoaded && finalNodeRead)
			{
				addFileData(pFileThread->getData());
				dataLoaded = false;
				finalNodeRead = false;
			}
	//	}
	}
	playbackThread->setStream(false);
}

void DataCoordinatorThread::sendDataChunk()
{
	while(dataRequests > 0)
	{
		if (chunkA->checkStatus() != FileReadAndChunk::depleted)
		{
			FileData data = chunkA->createChunk();
			if (playbackThread != nullptr)
			{
				playbackThread->setData(data);
			}
			dataRequests--;
		}
		else if (chunkB->checkStatus() != FileReadAndChunk::depleted)
		{
			FileReadAndChunk* temp = chunkA;
			chunkA = chunkB;
			chunkB = temp;
			FileData data = chunkA->createChunk();
			if (playbackThread != nullptr)
			{
				playbackThread->setData(data);
			}
			pFileThread->CreateData();
			dataRequests--;
		}
		else {
			addFileData(pFileThread->getData());
		}
	}
}
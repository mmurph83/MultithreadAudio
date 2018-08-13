#pragma once
#include "AudioFileReader.h"
#include "DataCoordinatorThread.h"
class DataCoordinatorThread;
class FileLoadThread
{
public:
	FileLoadThread(AudioFileReader reader);
	~FileLoadThread();
	void LaunchThread();
	DataCoordinatorThread* pDataCoord = nullptr;
	void CreateData()
	{
		createData = true;
	}
	FileData getData()
	{
		FileData returnData = data;
		data.fileData = nullptr;
		return returnData;
	}
private:
	AudioFileReader Reader;
	FileData data;
	bool createData = false;
};


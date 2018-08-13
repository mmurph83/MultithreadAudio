#include "FileLoadThread.h"



FileLoadThread::FileLoadThread(AudioFileReader reader) : Reader(reader)
{
	data.fileData = nullptr;
}


FileLoadThread::~FileLoadThread()
{
}

void FileLoadThread::LaunchThread()
{
	//FileData data;
	while (1)
	{
		if (createData)
		{
			if (!Reader.isDepleted())
			{
				if (data.fileData == nullptr)
				{
					data = Reader.CreateData();
					pDataCoord->signalDataLoad();
					createData = false;
					//pDataCoord->addFileData(Reader.CreateData());
				}
			}
			else
			{
				return;
			}
		}
		Sleep(20);
	}
}
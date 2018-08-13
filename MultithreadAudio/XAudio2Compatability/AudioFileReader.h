#pragma once
#include <fstream>
#include "FileData.h"


class AudioFileReader
{
public:
	AudioFileReader() = delete;
	AudioFileReader(std::string name, long readSize);
	~AudioFileReader();
	FileData CreateData();
	bool isDepleted()
	{
		if (waveNum > 22)
		{
			return true;
		}
		return false;
	}
private:
	long size;
	long currentPos = 0;
	long maxReadSize;
	int waveNum = 0;
	std::string fileName;
	bool depleted = false;
};


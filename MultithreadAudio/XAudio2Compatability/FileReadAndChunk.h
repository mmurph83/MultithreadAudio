#pragma once
#include "FileData.h"
#include <memory>
#include <stdlib.h>
class FileReadAndChunk
{
public:
	enum ReadStatus
	{
		reading,
		depleted
	};
	FileReadAndChunk() = delete;
	FileReadAndChunk( unsigned char* d, const long size, const long stride): data(d),maxSize(size), maxStride(stride)
	{
		data = (unsigned char*)malloc(sizeof(unsigned char)*600000);
		memcpy(data, d, size);
	}
	~FileReadAndChunk();
	void OverwriteChunk(unsigned char* d, const long size)
	{
		memcpy(data, d, size);
		free(d);
		maxSize = size;
		currentPos = 0;
		status = reading;
	}
	FileData createChunk()
	{
		FileData returnData;
		returnData.fileData = data + currentPos;
		long tempSize = maxSize - currentPos;
		if (tempSize < maxStride)
		{
			returnData.size = tempSize;
		}
		else
		{
			returnData.size = maxStride;
		}
		currentPos += maxStride;
		if (checkStatus() == depleted)
		{
			returnData.finalBuffer = true;
		}
		else
		{
			returnData.finalBuffer = false;
		}
		return returnData;
	}
	ReadStatus checkStatus()
	{
		if (currentPos >= maxSize)
		{
			status = depleted;
		}
		return status;
	}
private:
	unsigned char* data;
	long maxSize;
	long currentPos = 0;
	const long maxStride;
	ReadStatus status = reading;
};


#pragma once
struct FileData
{
	unsigned char* fileData;
	long size;
	bool finalBuffer = false;
};
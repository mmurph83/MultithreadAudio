#include "AudioFileReader.h"

AudioFileReader::AudioFileReader(std::string name, long readSize) :maxReadSize(readSize)
{

	FILE* pFile;
	fopen_s(&pFile,name.c_str(), "rb");
	fseek(pFile, 0, SEEK_END);
	size = 600000;
	rewind(pFile);
	fileName = name;
}
FileData AudioFileReader::CreateData()
{
	unsigned char* buff = (unsigned char*)malloc(sizeof(unsigned char)*size);
	FILE* pFile;
	char temp[50];
	sprintf(temp, "wave_%d.wav", waveNum++);
	fopen_s(&pFile, temp, "rb");
	//fseek(pFile, currentPos, SEEK_SET);
//	currentPos += maxReadSize;
	long copySize = fread(buff, 1, maxReadSize, pFile);
	printf("%d\n", copySize);
	FileData returnData;
	returnData.fileData = buff;
	returnData.size = copySize;
	return returnData;
}

AudioFileReader::~AudioFileReader()
{
}

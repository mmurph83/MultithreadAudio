#include "Audio.h"

#include "VoiceCallback.h"
#include "AudioFileReader.h"
#include "FileData.h"
#include "VoiceNodeThread.h"
#include "FileReadAndChunk.h"
#include "PlaybackThread.h"
#include "DataCoordinatorThread.h"
#include "FileLoadThread.h"
#include "WaveOutClass.h"
#include <thread>

class AudioFileReader;
class VoiceCallback;
struct FileData;

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg)
{
	if (uMsg == WOM_DONE)
	{
		printf("hi");
	}
}
XAUDIO2_BUFFER createBuffer(BYTE* data, long size)
{
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.LoopCount = 0;
	buffer.AudioBytes = size;  //buffer containing audio data
	buffer.pAudioData = data;
	//buffer.Flags = XAUDIO2_END_OF_STREAM;
	return buffer;
}
WAVEFORMATEX createFormat()
{
	WAVEFORMATEX wfx;
	wfx.nSamplesPerSec = 22050;

	/* sample size */
	wfx.wBitsPerSample = 16;

	/* channels*/
	wfx.nChannels = 2;

	return wfx;
}

int main() {
	
	HWAVEOUT hWaveOut;
	WAVEFORMATEX wfx;

	/*
	* first we need to set up the WAVEFORMATEX structure.
	* the structure describes the format of the audio.
	*/

	/* sample rate */
	int sampleRate = 22050;
	wfx.nSamplesPerSec = sampleRate;

	/* sample size */
	wfx.wBitsPerSample = 8;

	/* channels*/
	wfx.nChannels = 2;

	/*
	* WAVEFORMATEX also has other fields which need filling.
	* as long as the three fields above are filled this should
	* work for any PCM (pulse code modulation) format.
	*/

	/* size of _extra_ info */
	wfx.cbSize = 0; //sizeof(WAVEFORMATEX);
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	//wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample;
	//wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	/* device handle */
	//HWAVEOUT hWaveOut;

	/* for waveOut return values */
	MMRESULT result;

	/*
	* try to open the default wave device. WAVE_MAPPER is
	* a constant defined in mmsystem.h, it always points to the
	* default wave device on the system (some people have 2 or
	* more sound cards).
	*/

	UINT numDevices = waveOutGetNumDevs();
	numDevices;

	//result = waveOutOpen( ( LPHWAVEOUT ) &hWaveOut, WAVE_MAPPER, ( LPWAVEFORMATEX ) &wfx, ( DWORD ) ( UINT ) waveOutProc, NULL, CALLBACK_FUNCTION );

	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION); //WAVE_FORMAT_QUERY
	//=======ENVIRONMENT SET UP
	AudioFileReader reader("JetsonTheme_withHeader.wav", 512000);
	
	/*FILE* pFile = fopen("JetsonTheme_withHeader.wav", "rb");
	BYTE* buff;
	long size;
	if (pFile != nullptr)
	{
		//long size;
		fseek(pFile, 0, SEEK_END);
		size = ftell(pFile);
		rewind(pFile);
		fseek(pFile, 0, SEEK_SET);
		buff = (BYTE*)malloc(sizeof(BYTE)*size);
		long copySize = fread(buff, 1, 500000, pFile);
		printf("hi");
	}*/
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//HRESULT hr;
	//IXAudio2* pXAudio2 = NULL;
	//IXAudio2MasteringVoice * pMasterVoice;
	//Create XAudio2
	
	
	XAUDIO2_BUFFER buffer = { 0 };
	XAUDIO2_BUFFER buffer1 = { 0 };
	FileData fileData(reader.CreateData());
	buffer = createBuffer(fileData.fileData, fileData.size/2);
	//buffer1 = createBuffer(fileData.fileData + (fileData.size / 2), fileData.size / 2);
	//=======SETTING UP CALLBACK
	

	PlaybackThread* playbackThread = new PlaybackThread();
	/* size of _extra_ info */

	//pXAudio2->CreateSourceVoice(&pSourceVoice, &wfxx, 0, 1.0F, callback);
	//pSourceVoice->SubmitSourceBuffer(&buffer);
	//pSourceVoice->SubmitSourceBuffer(&buffer1);
	//pSourceVoice->SetSourceSampleRate(22050);
	//fileData = reader.CreateData();
	//FileReadAndChunk dataChunk(fileData.fileData, fileData.size, 500000/20);
	int x = 0;
	//while (dataChunk.checkStatus() != FileReadAndChunk::ReadStatus::depleted)
	DataCoordinatorThread* coordinatorThread = new DataCoordinatorThread();
	
	coordinatorThread->addFileData(fileData);
	fileData = reader.CreateData();
	coordinatorThread->addFileData(fileData);
	FileLoadThread* fileLoadThreadObject = new FileLoadThread(reader);
	std::thread fileloadthread(&FileLoadThread::LaunchThread, fileLoadThreadObject);
	fileLoadThreadObject->pDataCoord = coordinatorThread;
	coordinatorThread->pFileThread = fileLoadThreadObject;
	WaveOutClass* pWaveOutClass = new WaveOutClass(hWaveOut);
	pWaveOutClass->setPlayback(playbackThread);
	//fileData = reader.CreateData();
	while(x < 20)
	{
		//IXAudio2SourceVoice* pSourceVoice;
	//	VoiceCallback* callback = new VoiceCallback();
		//pXAudio2->CreateSourceVoice(&pSourceVoice, &wfxx, 0, 1.0F, callback);
		//fileData = dataChunk.createChunk();
		//buffer = createBuffer(fileData.fileData, fileData.size);
		VoiceNodeThread* t = new VoiceNodeThread(buffer,hWaveOut, pWaveOutClass);
		//callback->setThread(t);
		//pSourceVoice->Start();
		std::thread m(&VoiceNodeThread::launchThread, t);
		playbackThread->addThread(t, m, x++);
		t->pPlayback = playbackThread;
		//t->playAudio();
		//m.join();
	}
	playbackThread->lockVoiceNode();
	playbackThread->setCoordinatorThread(coordinatorThread);
	//coordinatorThread->sendDataChunk();
	std::thread n(&DataCoordinatorThread::launchThread, coordinatorThread);
	/*VoiceNodeThread* t = new VoiceNodeThread(buffer, pSourceVoice);
	callback->setThread(t);
	//pSourceVoice->Start();
	std::thread m(&VoiceNodeThread::launchThread,t);
	t->playAudio();
	m.join();*/
	//while(!callback.finished){}
	//callback.finished = false;
	//Sleep(2000);
	//=======CHANGING PAN
	playbackThread->startThread();
	//float pan = -1;
	//float outputMatrix[8];
	//for (int i = 0; i<8; i++) outputMatrix[i] = 0;
	// pan of -1.0 indicates all left speaker, 
	// 1.0 is all right speaker, 0.0 is split between left and right
	//float left = 0.5f - pan / 2;
	//float right = 0.5f + pan / 2;
	
	/*
	* BIG CHANGE between ver 7-8 - look in GetChannelMask
	*/

	while (1) {}
	//callback.finished = false;

	//delete pDataBuffer;
	
	CoUninitialize();
	return 0;
}
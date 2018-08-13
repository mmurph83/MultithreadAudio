#include "WaveOutClass.h"
static PlaybackThread* play;
static void CALLBACK waveProc(HWAVEOUT hwo, UINT uMsg, int x)
{
	if (uMsg == WOM_DONE)
	{
		play->notifyAudioCompletion();
		//pThread->endPlay();
	}
	if (uMsg == WOM_OPEN)
	{
		//printf("hi");
	}
}

WaveOutClass::WaveOutClass(HWAVEOUT waveOut): hWaveOut(waveOut)
{
	WAVEFORMATEX wfx;

	/*
	* first we need to set up the WAVEFORMATEX structure.
	* the structure describes the format of the audio.
	*/

	/* sample rate */
	int sampleRate = 22050;
	wfx.nSamplesPerSec = sampleRate;

	/* sample size */
	wfx.wBitsPerSample = 16;

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

	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveProc, 0, CALLBACK_FUNCTION); //WAVE_FORMAT_QUERY

}


WaveOutClass::~WaveOutClass()
{
	waveOutClose(hWaveOut);
}


void WaveOutClass::writeAudioBlock(WAVEHDR& header)
{
	lock.lock();
	play = pPlayback;
	/*
	* initialise the block header with the size
	* and pointer.
	*/
	//pThread = this;
	unsigned int hdrSize = sizeof(WAVEHDR);
	//WAVEHDR header;
	//ZeroMemory(&header, hdrSize);
	//header.dwBufferLength = size;
	//header.lpData = block;

	/*
	* prepare the block for playback
	*/
	while (waveOutUnprepareHeader(hWaveOut, &header, hdrSize) == WAVERR_STILLPLAYING);
	waveOutPrepareHeader(hWaveOut, &header, hdrSize);

	/*
	* write the block to the device. waveOutWrite returns immediately
	* unless a synchronous driver is used (not often).
	*/

	waveOutWrite(hWaveOut, &header, hdrSize);

	/*
	* wait a while for the block to play then start trying
	* to unprepare the header. this will fail until the block has
	* played.
	*/

	//Sleep(500);
	//while (waveOutUnprepareHeader(hWaveOut, &header, hdrSize) == WAVERR_STILLPLAYING);
	//endPlay();

	//waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	//Sleep(500);
	lock.unlock();
}
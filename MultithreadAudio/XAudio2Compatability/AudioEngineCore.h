#ifndef _AUDIO_ENGINE_CORE_H_
#define _AUDIO_ENGINE_CORE_H_

#include "AutoXA2Include.h"

/* - AudioEngineCore.h -
*  Core functions and macros for use
*  in the audio engine
*/

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

//Version difference: 2.8+ removed speaker macros??
#ifndef SPEAKER_STEREO
	#define SPEAKER_MONO             (SPEAKER_FRONT_CENTER)
	#define SPEAKER_STEREO           (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)
	#define SPEAKER_2POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY)
	#define SPEAKER_SURROUND         (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_CENTER)
	#define SPEAKER_QUAD             (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
	#define SPEAKER_4POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
	#define SPEAKER_5POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
	#define SPEAKER_7POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_FRONT_LEFT_OF_CENTER | SPEAKER_FRONT_RIGHT_OF_CENTER)
	#define SPEAKER_5POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT)
	#define SPEAKER_7POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_SIDE_LEFT  | SPEAKER_SIDE_RIGHT)
#endif

//Find wave audio data chunk
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

//Read wave data chunk
HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

//Get channel mask
DWORD GetChannelMask(IXAudio2 * pXAudio2, IXAudio2MasteringVoice* pMasteringVoice) {
	DWORD dwChannelMask = 0;
	
	/*VERSION DIFFERENCE:
	* 2.7 has device enumeration - get channel mask that way
	* 2.8+ removed device enumeration - use GetChannelMask
	*/
#if __XAUDIO2VER == 7
	//MasteringVoice not needed, reference to avoid compiler warning
	pMasteringVoice;

	UINT32 dCount = 0;
	HRESULT hr = pXAudio2->GetDeviceCount(&dCount);
	if (FAILED(hr)) return hr;

	for (UINT32 index = 0; index < dCount; ++index)
	{
		XAUDIO2_DEVICE_DETAILS details;
		hr = pXAudio2->GetDeviceDetails(index, &details);
		if (FAILED(hr))break;

		if (details.Role != XAUDIO2_DEVICE_ROLE::InvalidDeviceRole && details.Role != XAUDIO2_DEVICE_ROLE::NotDefaultDevice) {
			dwChannelMask = details.OutputFormat.dwChannelMask;
			break;
		}
	}
#else
	//XAudio2 env not needed, reference to avoid compiler warning
	pXAudio2;
	pMasteringVoice->GetChannelMask(&dwChannelMask);
#endif
	return dwChannelMask;
}


#endif

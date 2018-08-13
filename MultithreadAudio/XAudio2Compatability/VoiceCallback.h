#ifndef _VOICE_CALLBACK_H_
#define _VOICE_CALLBACK_H_

#include "AudioEngineCore.h"
#include "VoiceNodeThread.h"
#include <stdio.h>

/* - VoiceCallback.h -
* Sample voice callback
*/
class VoiceCallback : public IXAudio2VoiceCallback {
public:
	HANDLE hBufferEndEvent;
	bool finished = false;
	VoiceCallback() : hBufferEndEvent(CreateEvent(0, FALSE, FALSE, 0)) {}
	~VoiceCallback() {CloseHandle(hBufferEndEvent);}


	void STDMETHODCALLTYPE OnStreamEnd() {
		//finished = true;
		finished = true;
		if (pThread != nullptr)
		{
			pThread->endPlay();
		}
	}
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() { }
	void STDMETHODCALLTYPE  OnVoiceProcessingPassStart(UINT32 SamplesRequired) { SamplesRequired; }
	void STDMETHODCALLTYPE  OnBufferEnd(void * pBufferContext) { 
		pBufferContext;
	}
	void STDMETHODCALLTYPE  OnBufferStart(void * pBufferContext) { pBufferContext; }
	void STDMETHODCALLTYPE  OnLoopEnd(void * pBufferContext) {
		pBufferContext;
		finished = true;
		if (pThread != nullptr)
		{
			pThread->endPlay();
		}
	}
	void STDMETHODCALLTYPE  OnVoiceError(void * pBufferContext, HRESULT Error) { pBufferContext; Error; }
	void setThread(VoiceNodeThread* thread)
	{
		pThread = thread;
	}
private:
	VoiceNodeThread* pThread = nullptr;
};

#endif
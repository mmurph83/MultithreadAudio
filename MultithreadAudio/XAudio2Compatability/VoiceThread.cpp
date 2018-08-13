#include "VoiceThread.h"




VoiceThread::VoiceThread(XAUDIO2_BUFFER* buff, IXAudio2SourceVoice* voice): buffer(buff),pSourceVoice(voice)
{
	pSourceVoice->SubmitSourceBuffer(buffer);
}


VoiceThread::~VoiceThread()
{
}

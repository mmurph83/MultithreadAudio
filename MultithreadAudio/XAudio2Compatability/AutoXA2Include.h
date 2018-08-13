#ifndef _AUTO_XA2INCLUDE_H_
#define _AUTO_XA2INCLUDE_H_

#include "vcsversion.h"

/* - AutoXA2Include.h - 
*  Used by AudioEngineCore to
*  include correct XAudio2 version
*/


#ifdef __XAUDIO2VER
#if __XAUDIO2VER == 7
#include "..\dxJun2010\Include\XAudio2.h"
#else 
#include <XAudio2.h>
#pragma comment(lib,"XAudio2.lib")
#endif
#else
#include "..\dxJun2010\Include\XAudio2.h"
#endif

#endif
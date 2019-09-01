/*=============================================================================
SampleSound.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SAMPLESOUND_H_
#define _SAMPLESOUND_H_

#include <Core/Core.h>
#include <Audio/Audio.h>
#include <Input/Input.h>
#include <EntryPoint.h>
#include <Plugins.h>

#include <Platforms/Win32/Win32DebugLogHandler.h>

typedef SonataEngine::byte SEbyte;

using namespace SonataEngine;
using namespace SonataEngine::Audio;

#ifdef SE_STATIC
#	include <AudioSystem_DS8/DS8AudioSystemLib.h>
#	include <Sound_WAV/WAVSoundPlugin.h>
#else
#endif

#endif 

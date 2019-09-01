/*=============================================================================
DS8Sound.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DS8Sound.h"

namespace SE_DS8
{

DS8Sound::DS8Sound() :
	Sound(),
	_AudioSystem(NULL),
	_Buffer(NULL),
	_Length(0)
{
}

DS8Sound::~DS8Sound()
{
	SE_DELETE_ARRAY(_Buffer);
}

bool DS8Sound::Create(DS8AudioSystem* audioSystem, const AudioFormat& format, SEbyte* buffer, int32 sampleCount, SoundMode mode)
{
	_AudioSystem = audioSystem;
	_Format = format;
	_Buffer = buffer;
	_SampleCount = sampleCount;
	_Mode = mode;

	return true;
}

}

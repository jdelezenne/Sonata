/*=============================================================================
AudioDevice.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AudioDevice.h"

namespace SonataEngine
{
	namespace Audio
	{
		AudioDeviceCaps::AudioDeviceCaps() :
			_MinSampleRate(0),
			_MaxSampleRate(0),
			_SpeakerMode(SpeakerMode_DirectOut)
		{
		}

		AudioDeviceCaps::AudioDeviceCaps(int32 minSampleRate, int32 maxSampleRate, SpeakerMode mode) :
			_MinSampleRate(minSampleRate),
			_MaxSampleRate(maxSampleRate),
			_SpeakerMode(mode)
		{
		}

		AudioDeviceCaps::~AudioDeviceCaps()
		{
		}


		AudioDevice::AudioDevice() :
			_Index(0),
			_AudioDeviceType(AudioDeviceType_Playback)
		{
		}

		AudioDevice::AudioDevice(int32 index, AudioDeviceType type, const String& description, const AudioDeviceCaps& caps) :
			_Index(index),
			_AudioDeviceType(type),
			_Description(description),
			_Caps(caps)
		{
		}

		AudioDevice::~AudioDevice()
		{
		}
	}
}

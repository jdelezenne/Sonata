/*=============================================================================
AudioDevice.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIODEVICE_H_
#define _SE_AUDIODEVICE_H_

#include "Core/Core.h"
#include "Audio/Common.h"

namespace SonataEngine
{
	namespace Audio
	{
		/**
			@brief Audio device capabilities.
		*/
		class SE_AUDIO_EXPORT AudioDeviceCaps
		{
		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			AudioDeviceCaps();

			/** Constructor. */
			AudioDeviceCaps(int32 minSampleRate, int32 maxSampleRate, SpeakerMode mode  = SpeakerMode_Stereo);

			/** Destructor. */
			virtual ~AudioDeviceCaps();
			//@}

			/** @name Properties. */
			//@{
			/** Retrieves the maximum sample rate that is supported. */
			int32 GetMinSampleRate() const { return _MinSampleRate; }

			/** Retrieves the minimum sample rate that is supported. */
			int32 GetMaxSampleRate() const { return _MaxSampleRate; }

			/** Retrieves the speaker mode. */
			SpeakerMode GetSpeakerMode() const { return _SpeakerMode; }
			//@}

		protected:
			int32 _MinSampleRate;
			int32 _MaxSampleRate;
			SpeakerMode _SpeakerMode;
		};

		/**
			@brief Audio device.
		*/
		class SE_AUDIO_EXPORT AudioDevice : public RefObject
		{
		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			AudioDevice();

			/** Constructor. */
			AudioDevice(int32 index, AudioDeviceType type, const String& description, const AudioDeviceCaps& caps);

			/** Destructor. */
			virtual ~AudioDevice();
			//@}

			/** @name Properties. */
			//@{
			/** Retrieves the index of the device. */
			int32 GetIndex() const { return _Index; }

			/** Retrieves the type of the device. */
			AudioDeviceType GetAudioDeviceType() const { return _AudioDeviceType; }

			/** Retrieves a textual description of the device. */
			String GetDescription() const { return _Description; }

			/** Retrieves the capabilities of the device. */
			AudioDeviceCaps GetCaps() const { return _Caps; }
			//@}

		protected:
			int32 _Index;
			AudioDeviceType _AudioDeviceType;
			String _Description;
			AudioDeviceCaps _Caps;
		};
	}
}

#endif 

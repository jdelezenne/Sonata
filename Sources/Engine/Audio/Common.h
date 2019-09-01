/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_COMMON_H_
#define _SE_AUDIO_COMMON_H_

#include <Core/Core.h>

#define SE_ID_AUDIOSYSTEM SonataEngine::SE_ID(0x5c25dbeb,0xbec04570)

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_AUDIO_EXPORT
#	else
#		ifdef SE_ENGINEAUDIO
#			define SE_AUDIO_EXPORT __declspec(dllexport)
#		else
#			define SE_AUDIO_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_AUDIO_EXPORT
#endif

namespace SonataEngine
{
	namespace Audio
	{
		enum SpeakerMode
		{
			/**
				The audio is passed through directly, without being configured for
				speakers.
			*/
			SpeakerMode_DirectOut,

			/** The audio is played through headphones. */
			SpeakerMode_Headphone,

			/** The audio is played through a single speaker. */
			SpeakerMode_Mono,

			/** The audio is played through stereo speakers (default value). */
			SpeakerMode_Stereo,

			/** The audio is played through quadraphonic speakers. */
			SpeakerMode_4Point1,

			/**
				The audio is played through a speaker arrangement of five
				surround speakers with a subwoofer.
			*/
			SpeakerMode_5Point1,

			/**
				The audio is played through a speaker arrangement of seven surround
				speakers with a subwoofer.
			*/ 
			SpeakerMode_7Point1,

			/** The audio is played through surround speakers. */
			SpeakerMode_Surround
		};

		/** Audio device types. */
		enum AudioDeviceType
		{
			/** Audio device for playback. */
			AudioDeviceType_Playback,

			/** Audio device for recording. */
			AudioDeviceType_Record
		};

		/** 3D Sound modes. */
		enum SoundMode3D
		{
			/** Normal processing. This is the default mode. */
			SoundMode3D_Normal,

			/**
				Sound parameters (position, velocity, and orientation)
				are relative to the listener's parameters.
			*/
			SoundMode3D_HeadRelative,

			/**
				Processing of 3-D sound is disabled. The sound seems to
				originate from the center of the listener's head.
			*/
			SoundMode3D_Disabled
		};

		enum SoundPlayState
		{
			SoundPlayState_Play,
			SoundPlayState_Pause,
			SoundPlayState_Stop
		};

		/** Audio format types. */
		enum AudioFormatType
		{
			AudioFormatType_Unknown,
			AudioFormatType_PCM8,
			AudioFormatType_PCM16,
			AudioFormatType_PCM24,
			AudioFormatType_PCM32,
			AudioFormatType_Compressed
		};

		/** Audio format. */
		struct AudioFormat
		{
			AudioFormat() :
				FormatType(AudioFormatType_Unknown),
				Channels(2),
				SamplesPerSecond(44100)
			{
			}

			AudioFormat(AudioFormatType format, int32 channels, int32 samplesPerSecond) :
				FormatType(format),
				Channels(channels),
				SamplesPerSecond(samplesPerSecond)
			{
			}

			/**
				Retrieves the average data-transfer rate, in bytes per second,
				for the format type.
			*/
			int32 GetAverageBytesPerSecond() const
			{
				return (SamplesPerSecond * GetBlockAlign());
			}

			/** Retrieves the bits per sample for the format type. */
			int32 GetBitsPerSample() const
			{
				switch (FormatType)
				{
				case AudioFormatType_PCM8: return 8;
				case AudioFormatType_PCM16: return 16;
				case AudioFormatType_PCM24: return 24;
				case AudioFormatType_PCM32: return 32;

				case AudioFormatType_Unknown:
				case AudioFormatType_Compressed:
				default:
					return 0;
				}
			}
			
			/** Retrieves the minimum atomic unit of data, in bytes, for the format type. */
			int32 GetBlockAlign() const
			{
				return (GetBitsPerSample() / 8 * Channels);
			}

			/** Retrieves and sets the waveform-audio format type, for the format type. */
			AudioFormatType FormatType;

			/**
				Retrieves and sets the number of channels in the waveform-audio data,
				for the format type.
			*/
			int32 Channels;

			/**
				Retrieves and sets the sample rate, in samples per second (hertz),
				for the format type.
			*/
			int32 SamplesPerSecond;
		};

		/** Sound buffer modes. */
		enum SoundMode
		{
			/** Default settings: sample, 2D, hardware. */
			SoundMode_Default = 0,

			/** No 3D capabilities. */
			SoundMode_2D = (1<<0),

			/** 3D capabilities. */
			SoundMode_3D = (1<<1),

			/** The buffer try to use hardware mixing and if failed try software mixing. */
			SoundMode_Hardware = (1<<2),

			/** The buffer must use hardware mixing. */
			SoundMode_ForceHardware = (1<<3),

			/** The buffer must use software mixing. */
			SoundMode_Software = (1<<4),

			/** Global sound buffer. */
			SoundMode_Global = (1<<5),

			/** Static sample. */
			SoundMode_Sample = (1<<6),

			/** Streamed buffer. */
			SoundMode_Stream = (1<<7)
		};

		/** EAX Reverb environment presets. */
		enum AudioEnvironment
		{
			AudioEnvironment_Off,
			AudioEnvironment_Generic,
			AudioEnvironment_PaddedCell,
			AudioEnvironment_Room,
			AudioEnvironment_BathRoom,
			AudioEnvironment_LivinGRoom,
			AudioEnvironment_StoneRoom,
			AudioEnvironment_Auditorium,
			AudioEnvironment_ConcertHall,
			AudioEnvironment_Cave,
			AudioEnvironment_Arena,
			AudioEnvironment_Hangar,
			AudioEnvironment_CarpetedHallway,
			AudioEnvironment_Hallway,
			AudioEnvironment_StoneCorridor,
			AudioEnvironment_Alley,
			AudioEnvironment_Forest,
			AudioEnvironment_City,
			AudioEnvironment_Mountains,
			AudioEnvironment_Quarry,
			AudioEnvironment_Plain,
			AudioEnvironment_ParkingLot,
			AudioEnvironment_SewerPipe,
			AudioEnvironment_UnderWater,
			AudioEnvironment_Drugged,
			AudioEnvironment_Dizzy,
			AudioEnvironment_Psychotic
		};
	}
}

#endif

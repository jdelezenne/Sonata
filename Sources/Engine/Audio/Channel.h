/*=============================================================================
Channel.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_CHANNEL_H_
#define _SE_AUDIO_CHANNEL_H_

#include "Core/Core.h"
#include "Audio/Common.h"

namespace SonataEngine
{
	namespace Audio
	{
		class AudioSystem;
		class ChannelGroup;

		/**
			@brief Audio channel.

			A channel provides methods for playing and recording sounds.
		*/
		class SE_AUDIO_EXPORT Channel : public RefObject
		{
		public:
			/** Destructor. */
			virtual ~Channel();

			/** @name Properties. */
			//@{
			/** Retrieves or sets the channel group of the channel. */
			ChannelGroup* GetGroup() const { return _Group; }
			void SetGroup(ChannelGroup* value);

			/** Retrieves or sets the frequency, in samples per second, of the channel. */
			real32 GetFrequency() const { return _Frequency; }
			void SetFrequency(real32 value) { _Frequency = value; }

			/** Retrieves or sets the volume of the channel. */
			real32 GetVolume() const { return _volume; }
			void SetVolume(real32 value) { _volume = value; }

			/** Retrieves or sets the pan position of the channel. */
			real32 GetPan() const { return _Pan; }
			void SetPan(real32 value) { _Pan = value; }

			/** Retrieves or sets the play state of the sound. */
			SoundPlayState GetSoundPlayState() const { return _SoundPlayState; }
			void SetSoundPlayState(SoundPlayState value) { _SoundPlayState = value; }

			/** Retrieves or sets the current playback position of the channel. */
			int32 GetPlayPosition() const { return _PlayPosition; }
			void SetPlayPosition(int32 value) { _PlayPosition = value; }

			/** Mutes or unmute the channel. */
			bool GetMute() const { return _Mute; }
			void SetMute(bool value) { _Mute = value; }
			//@}

			/** @name 3D Properties. */
			//@{
			/** Retrieves or sets the 3D sound processing mode to be set. */
			SoundMode3D GetMode() const { return _Mode; }
			void SetMode(SoundMode3D value) { _Mode = value; }

			/** Retrieves or sets the current position of the 3D sound buffer. */
			const Vector3& GetPosition() const { return _Position; }
			void SetPosition(const Vector3& value) { _Position = value; }

			/** Retrieves or sets the current velocity of the 3D sound buffer. */
			const Vector3& GetVelocity() const { return _Velocity; }
			void SetVelocity(const Vector3& value) { _Velocity = value; }

			/** Retrieves or sets the minimum attenuation distance. */
			real32 GetMinDistance() const { return _MinDistance; }
			void SetMinDistance(real32 value) { _MinDistance = value; }

			/** Retrieves or sets the maximum attenuation distance. */
			real32 GetMaxDistance() const { return _MaxDistance; }
			void SetMaxDistance(real32 value) { _MaxDistance = value; }

			/** Retrieves or sets the angle of the inside sound projection cone. */
			int32 GetInsideConeAngle() const { return _InsideConeAngle; }
			void SetInsideConeAngle(int32 value) { _InsideConeAngle = value; }

			/** Retrieves or sets the angle of the outside sound projection cone. */
			int32 GetOutsideConeAngle() const { return _OutsideConeAngle; }
			void SetOutsideConeAngle(int32 value) { _OutsideConeAngle = value; }

			/**
				Retrieves or sets the volume of the sound outside the outside
				angle of the sound projection cone.
			*/
			int32 GetConeOutsideVolume() const { return _ConeOutsideVolume; }
			void SetConeOutsideVolume(int32 value) { _ConeOutsideVolume = value; }

			/** Retrieves or sets the current orientation of this 3D buffer's sound projection cone. */
			const Vector3& GetConeOrientation() const { return _ConeOrientation; }
			void SetConeOrientation(const Vector3& value) { _ConeOrientation = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Starts playback or recording. */
			virtual void Play() = 0;

			/** Pauses playback or recording. */
			virtual void Pause() = 0;

			/** Stops playback or recording. */
			virtual void Stop() = 0;
			//@}

		protected:
			/** Constructor. */
			Channel();

		protected:
			ChannelGroup* _Group;
			real32 _Frequency;
			real32 _volume;
			real32 _Pan;
			SoundPlayState _SoundPlayState;
			int32 _PlayPosition;
			bool _Mute;

			SoundMode3D _Mode;
			Vector3 _Position;
			Vector3 _Velocity;
			real32 _MinDistance;
			real32 _MaxDistance;
			int32 _InsideConeAngle;
			int32 _OutsideConeAngle;
			int32 _ConeOutsideVolume;
			Vector3 _ConeOrientation;

			friend ChannelGroup;
		};

		typedef SmartPtr<Channel> ChannelPtr;
	}
}

#endif 

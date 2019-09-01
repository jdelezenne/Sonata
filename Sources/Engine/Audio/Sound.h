/*=============================================================================
Sound.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_SOUND_H_
#define _SE_AUDIO_SOUND_H_

#include "Core/Core.h"
#include "Audio/Common.h"

namespace SonataEngine
{
	namespace Audio
	{
		class AudioSystem;

		/**
			@brief Sound.

			@todo Add time units.
		*/
		class SE_AUDIO_EXPORT Sound : public RefObject
		{
		public:
			/** Destructor. */
			virtual ~Sound();

			/** @name Properties. */
			//@{
			/** Retrieves the format of the sound. */
			AudioFormat GetFormat() const { return _Format; }

			/** Retrieves the length of the sound. */
			virtual int32 GetLength() const = 0;

			/**
				Retrieves or sets the number of times the sound will loop.
				@remarks
					-1 = infinite loop.
					0 = play once.
					1 = loop once.
					Default = 0.
			*/
			int32 GetLoopCount() const { return _LoopCount; }
			void SetLoopCount(int32 value);

			/** Returns whether the sound is looped. */
			bool IsLooped() const { return _LoopCount > 0; }

			/**
				Retrieves or sets the loop points of the sound.
				@remarks
					Unsupported with some Audio systems such as DirectSound
					which will only loop the whole sound.
			*/
			void GetLoopStart(Array<int32>& value) const { value = _LoopStart; }
			void SetLoopStart(const Array<int32>& value) { _LoopStart = value; }

			void GetLoopEnd(Array<int32>& value) const { value = _LoopEnd; }
			void SetLoopEnd(const Array<int32>& value) { _LoopEnd = value; }
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
			Vector3 GetVelocity() const { return _Velocity; }
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

		protected:
			/** Constructor. */
			Sound();

		protected:
			AudioFormat _Format;
			int32 _LoopCount;
			Array<int32> _LoopStart;
			Array<int32> _LoopEnd;

			SoundMode3D _Mode;
			Vector3 _Position;
			Vector3 _Velocity;
			real32 _MinDistance;
			real32 _MaxDistance;
			int32 _InsideConeAngle;
			int32 _OutsideConeAngle;
			int32 _ConeOutsideVolume;
			Vector3 _ConeOrientation;
		};

		typedef SmartPtr<Sound> SoundPtr;
	}
}

#endif 

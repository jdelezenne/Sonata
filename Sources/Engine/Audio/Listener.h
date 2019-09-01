/*=============================================================================
Listener.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_LISTENER_H_
#define _SE_AUDIO_LISTENER_H_

#include "Core/Core.h"
#include "Audio/Common.h"

namespace SonataEngine
{
	namespace Audio
	{
		/**
			@brief Audio listener.
		*/
		class SE_AUDIO_EXPORT Listener : public RefObject
		{
		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Listener();

			/** Destructor. */
			virtual ~Listener();
			//@}

			/** @name Properties. */
			//@{
			/** Retrieves and sets the listener's position. */
			Vector3 GetPosition() const { return _Position; }
			void SetPosition(Vector3 value) { _Position = value; }

			/** Retrieves and sets the listener's velocity. */
			Vector3 GetVelocity() const { return _Velocity; }
			void SetVelocity(Vector3 value) { _Velocity = value; }

			/** Retrieves and sets the listener's front orientation. */
			Vector3 GetOrientationFront() const { return _OrientationFront; }
			void SetOrientationFront(Vector3 value) { _OrientationFront = value; }

			/** Retrieves and sets the listener's top orientation. */
			Vector3 GetOrientationTop() const { return _OrientationTop; }
			void SetOrientationTop(Vector3 value) { _OrientationTop = value; }

			/** Retrieves and sets the ratio of Doppler effect to that in the real world. */
			real32 GetDopplerFactor() const { return _DopplerFactor; }
			void SetDopplerFactor(real32 value) { _DopplerFactor = value; }

			/** Retrieves and sets the number of meters in a vector unit. */
			real32 GetDistanceFactor() const { return _DistanceFactor; }
			void SetDistanceFactor(real32 value) { _DistanceFactor = value; }

			/** Retrieves and sets the ratio of attenuation over distance to that in the real world. */
			real32 GetRolloffFactor() const { return _RolloffFactor; }
			void SetRolloffFactor(real32 value) { _RolloffFactor = value; }
			//@}

		protected:
			Vector3 _Position;
			Vector3 _Velocity;
			Vector3 _OrientationFront;
			Vector3 _OrientationTop;
			real32 _DopplerFactor;
			real32 _DistanceFactor;
			real32 _RolloffFactor;
		};

		typedef SmartPtr<Listener> ListenerPtr;
	}
}

#endif 

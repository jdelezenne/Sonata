/*=============================================================================
Sound.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Sound.h"

namespace SonataEngine
{
	namespace Audio
	{
		Sound::Sound() :
			RefObject(),
			_LoopCount(0),
			_Mode(SoundMode3D_Normal),
			_Position(Vector3::Zero),
			_Velocity(Vector3::Zero),
			_MinDistance(1.0f),
			_MaxDistance(10000.0f),
			_InsideConeAngle(0),
			_OutsideConeAngle(360),
			_ConeOutsideVolume(0),
			_ConeOrientation(Vector3::Zero)
		{
		}

		Sound::~Sound()
		{
		}

		void Sound::SetLoopCount(int32 value)
		{
			_LoopCount = value;
			if (_LoopCount < -1)
				_LoopCount = -1;
		}
	}
}

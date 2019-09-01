/*=============================================================================
Channel.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Channel.h"
#include "Audio/ChannelGroup.h"
#include "Audio/AudioSystem.h"

namespace SonataEngine
{
	namespace Audio
	{
		Channel::Channel() :
			RefObject(),
			_Group(NULL),
			_Frequency(0.0f),
			_volume(1.0f),
			_Pan(0.0f),
			_SoundPlayState(SoundPlayState_Stop),
			_Mute(false),
			_PlayPosition(0),
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

		Channel::~Channel()
		{
		}

		void Channel::SetGroup(ChannelGroup* value)
		{
			if (value == NULL)
			{
				SEthrow(ArgumentNullException("value"));
				return;
			}

			if (_Group != NULL)
			{
				_Group->RemoveChannel(this);
			}

			_Group = value;
		}
	}
}

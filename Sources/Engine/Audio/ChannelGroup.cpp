/*=============================================================================
ChannelGroup.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ChannelGroup.h"
#include "Audio/AudioSystem.h"

namespace SonataEngine
{
	namespace Audio
	{
		ChannelGroup::ChannelGroup(AudioSystem* audioSystem) :
			RefObject(),
			_audioSystem(audioSystem),
			_volume(1.0f)
		{
		}

		ChannelGroup::~ChannelGroup()
		{
			RemoveAllChannels();
		}

		int ChannelGroup::GetChannelCount() const
		{
			return _channels.Count();
		}

		bool ChannelGroup::AddChannel(Channel* channel)
		{
			if (channel == NULL)
			{
				SEthrow(ArgumentNullException("channel"));
				return false;
			}
			else
			{
				if (!_channels.Contains(channel))
				{
					channel->_Group = this;
					_channels.Add(channel);
				}

				return true;
			}
		}

		bool ChannelGroup::RemoveChannel(Channel* channel)
		{
			if (channel == NULL)
			{
				SEthrow(ArgumentNullException("channel"));
				return false;
			}
			else
			{
				channel->_Group = _audioSystem->GetMasterChannelGroup();
				_channels.Remove(channel);
				return true;
			}
		}

		void ChannelGroup::RemoveAllChannels()
		{
			ChannelList::Iterator it = _channels.GetIterator();
			while (it.Next())
			{
				it.Current()->_Group = _audioSystem->GetMasterChannelGroup();
			}

			_channels.Clear();
		}

		Channel* ChannelGroup::GetChannel(int index) const
		{
			if (index < 0 || index >= _channels.Count())
				return NULL;

			return _channels[index];
		}

		void ChannelGroup::Play()
		{
			ChannelList::Iterator it = _channels.GetIterator();
			while (it.Next())
			{
				it.Current()->Play();
			}
		}

		void ChannelGroup::Pause()
		{
			ChannelList::Iterator it = _channels.GetIterator();
			while (it.Next())
			{
				it.Current()->Pause();
			}
		}

		void ChannelGroup::Stop()
		{
			ChannelList::Iterator it = _channels.GetIterator();
			while (it.Next())
			{
				it.Current()->Stop();
			}
		}
	}
}

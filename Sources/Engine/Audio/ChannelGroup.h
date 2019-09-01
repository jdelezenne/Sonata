/*=============================================================================
ChannelGroup.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIO_CHANNELGROUP_H_
#define _SE_AUDIO_CHANNELGROUP_H_

#include "Core/Core.h"
#include "Audio/Common.h"
#include "Audio/Channel.h"

namespace SonataEngine
{
	namespace Audio
	{
		class AudioSystem;

		/**
			@brief Audio channel group.

			A channel group provides methods for managing a group of channels.
			There is at least one channel group, the master channel.
			When a channel is removed from a channel, it is automatically added
			to the master channel.
			Adding a channel to a channel group removes it from its previous group.
		*/
		class SE_AUDIO_EXPORT ChannelGroup : public RefObject
		{
		public:
			typedef Array<Channel*> ChannelList;

			/** Destructor. */
			virtual ~ChannelGroup();

			/** @name Properties. */
			//@{
			/** Retrieves or sets the name of the channel group. */
			String GetName() const { return _name; }
			void SetName(String value) { _name = value; }

			/** Retrieves or sets the volume of the channel group. */
			real32 GetVolume() const { return _volume; }
			void SetVolume(real32 value) { _volume = value; }
			//@}

			/** @name Channels. */
			//@{
			int GetChannelCount() const;
			bool AddChannel(Channel* channel);
			bool RemoveChannel(Channel* channel);
			void RemoveAllChannels();
			Channel* GetChannel(int index) const;
			//@}

			/** @name Operations. */
			//@{
			/** Starts playback or recording. */
			void Play();

			/** Pauses playback or recording. */
			void Pause();

			/** Stops playback or recording. */
			void Stop();
			//@}

		private:
			/** Constructor. */
			ChannelGroup(AudioSystem* audioSystem);

		protected:
			AudioSystem* _audioSystem;
			String _name;
			real32 _volume;
			ChannelList _channels;

			friend AudioSystem;
		};

		typedef SmartPtr<ChannelGroup> ChannelGroupPtr;
	}
}

#endif 

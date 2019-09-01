/*=============================================================================
AudioSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AudioSystem.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Audio/AudioDevice.h"
#include "Audio/ChannelGroup.h"
#include "Audio/SoundReader.h"
#include "Audio/SoundWriter.h"

namespace SonataEngine
{
namespace Audio
{

AudioSystem::AudioSystem() :
	Manager(),
	_streamBufferSize(DefaultStreamBufferSize),
	_listener(new Listener())
{
}

AudioSystem::~AudioSystem()
{
	SE_DELETE(_listener);
}

int AudioSystem::GetAudioDeviceCount() const
{
	return _audioDevices.Count();
}

int AudioSystem::GetPlaybackDeviceCount() const
{
	int count = 0;
	AudioDeviceList::Iterator it = _audioDevices.GetIterator();
	while (it.Next())
	{
		if (it.Current().GetAudioDeviceType() == AudioDeviceType_Playback)
		{
			count++;
		}
	}

	return count;
}

int AudioSystem::GetRecordDeviceCount() const
{
	int count = 0;
	AudioDeviceList::Iterator it = _audioDevices.GetIterator();
	while (it.Next())
	{
		if (it.Current().GetAudioDeviceType() == AudioDeviceType_Record)
		{
			count++;
		}
	}

	return count;
}

AudioSystem::AudioDeviceList::Iterator AudioSystem::GetAudioDeviceIterator() const
{
	return _audioDevices.GetIterator();
}

const AudioDevice* AudioSystem::GetAudioDevice(int index) const
{
	if (index < 0 || index >= _audioDevices.Count())
		return NULL;

	return &_audioDevices[index];
}

const AudioDevice* AudioSystem::GetPlaybackDevice(int index) const
{
	int count = _audioDevices.Count();
	int i, j;
	j = -1;
	for (i=0; i<count; i++)
	{
		if (_audioDevices[i].GetAudioDeviceType() == AudioDeviceType_Playback)
		{
			j++;
			if (j == index)
			{
				return &_audioDevices[i];
			}
		}
	}

	return NULL;
}

const AudioDevice* AudioSystem::GetRecordDevice(int index) const
{
	int count = _audioDevices.Count();
	int i, j;
	j = -1;
	for (i=0; i<count; i++)
	{
		if (_audioDevices[i].GetAudioDeviceType() == AudioDeviceType_Record)
		{
			j++;
			if (j == index)
			{
				return &_audioDevices[i];
			}
		}
	}

	return NULL;
}

ChannelGroup* AudioSystem::CreateChannelGroup()
{
	return new ChannelGroup(this);
}

Sound* AudioSystem::OpenSoundSample(SoundReader& reader, SoundMode mode)
{
	AudioFormat format;
	if (!reader.GetFormat(format))
		return NULL;

	Stream* stream = reader.GetStream();
	if (stream == NULL)
		return NULL;

	int32 length = reader.GetLength();
	int32 bufferSize = length * format.GetBlockAlign();
	SEbyte* buffer = new SEbyte[bufferSize];
	if (reader.ReadSound(buffer, bufferSize) != bufferSize)
		return NULL;

	Sound* sound = CreateSoundSample(format, buffer, length, mode);

	return sound;
}

Sound* AudioSystem::OpenSoundSample(const String& source, SoundMode mode)
{
	File* file = new File(source);
	if (file == NULL)
		return NULL;

	FileStreamPtr stream = file->Open(FileMode_Open, FileAccess_Read, FileShare_Read);
	if (stream == NULL)
	{
		delete file;
		return NULL;
	}

	delete file;
}

Sound* AudioSystem::OpenSoundStream(SoundReader& reader, SoundMode mode)
{
	AudioFormat format;
	if (!reader.GetFormat(format))
		return NULL;

	Stream* stream = reader.GetStream();
	if (stream == NULL)
		return NULL;

	return CreateSoundStream(format, *stream, mode);
}

}
}

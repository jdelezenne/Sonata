/*=============================================================================
AudioSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AUDIOSYSTEM_H_
#define _SE_AUDIOSYSTEM_H_

#include "Core/Core.h"
#include "Audio/Common.h"
#include "Audio/AudioDevice.h"
#include "Audio/Listener.h"
#include "Audio/Sound.h"
#include "Audio/Channel.h"

namespace SonataEngine
{
namespace Audio
{

class ChannelGroup;
class DSP;
class SoundReader;
class SoundWriter;

const int32 DefaultStreamBufferSize = 0x4000; //16384

/** Audio system description. */
struct AudioSystemDescription
{
	AudioSystemDescription() :
		UsePlayback(true),
		UseRecord(false),
		PlaybackDevice(0),
		RecordDevice(0),
		DefaultFormat(true),
		Format(AudioFormatType_PCM16, 2, 22050),
		UseEAX(false),
		Window(NULL),
		ExclusiveLevel(true)
	{
	}

	bool UsePlayback;
	bool UseRecord;
	int32 PlaybackDevice;
	int32 RecordDevice;
	bool DefaultFormat;
	AudioFormat Format;
	bool UseEAX;
	Window* Window;
	bool ExclusiveLevel;
};

/**
	@brief Base class for audio system implementations.
*/
class SE_AUDIO_EXPORT AudioSystem : public Manager, public Context<AudioSystem>
{
public:
	typedef BaseArray<AudioDevice> AudioDeviceList;

	/** @name Constructor / Destructor. */
	//@{
	/** Constructor. */
	AudioSystem();

	/** Destructor. */
	virtual ~AudioSystem();
	//@}

	/** @name Properties. */
	//@{
	/**
		Retrieves and sets the internal buffer size for streamable sounds, in bytes.
		@remarks Default is 16384 bytes;
	*/
	int32 GetStreamBufferSize() const { return _streamBufferSize; }
	void SetStreamBufferSize(int32 value) { _streamBufferSize = value; }

	int32 GetPlaybackDevice() const { return _audioSystemDescription.PlaybackDevice; }
	void SetPlaybackDevice(int32 value) { _audioSystemDescription.PlaybackDevice = value; }

	int32 GetRecordDevice() const { return _audioSystemDescription.RecordDevice; }
	void SetRecordDevice(int32 value) { _audioSystemDescription.RecordDevice = value; }

	/** Retrieves and sets the listener. */
	Listener* GetListener() const { return _listener; }
	void SetListener(Listener* value) { _listener = value; }

	/** Retrieves the master channel group. */
	ChannelGroup* GetMasterChannelGroup() const { return _masterChannelGroup; }
	//@}

	/** @name Audio Devices. */
	//@{
	int GetAudioDeviceCount() const;
	int GetPlaybackDeviceCount() const;
	int GetRecordDeviceCount() const;
	AudioDeviceList::Iterator GetAudioDeviceIterator() const;
	const AudioDevice* GetAudioDevice(int index) const;
	const AudioDevice* GetPlaybackDevice(int index) const;
	const AudioDevice* GetRecordDevice(int index) const;
	//@}

	/** Creates a channel group. */
	ChannelGroup* CreateChannelGroup();

	/**
		Enumerates the available devices.
		@return true if successful; otherwise, false.
	*/
	virtual bool EnumerateDevices() = 0;

	/** @name Creation / Destruction. */
	//@{
	/** Creates the audio system with the specified parameters. */
	virtual bool Create(const AudioSystemDescription& desc) = 0;

	/** Destroys the audio system. */
	virtual void Destroy() = 0;
	//@}

	/** Updates the audio system. */
	virtual void Update() = 0;
	//@}

	/** @name Sounds. */
	//@{
	Sound* OpenSoundSample(SoundReader& reader, SoundMode mode = SoundMode_Default);
	Sound* OpenSoundSample(const String& source, SoundMode mode = SoundMode_Default);

	Sound* OpenSoundStream(SoundReader& reader, SoundMode mode = SoundMode_Default);
	Sound* OpenSoundStream(const String& source, SoundMode mode = SoundMode_Default);

	/** Creates a static sound. */
	virtual Sound* CreateSoundSample(const AudioFormat& format, SEbyte* buffer, int32 sampleCount, SoundMode mode = SoundMode_Default) = 0;

	/** Creates a streamed sound. */
	virtual Sound* CreateSoundStream(const AudioFormat& format, Stream& stream, SoundMode mode = SoundMode_Default) = 0;

	/** Destroys a sound. */
	virtual void DestroySound(Sound* sound) = 0;

	/** Creates a channel. */
	virtual Channel* CreateChannel(Sound* sound) = 0;

	/** Destroys a channel. */
	virtual void DestroyChannel(Channel* channel) = 0;

	/** Starts playback or recording of a channel. */
	virtual void Play(Channel* channel) = 0;

	/** Pauses playback or recording of a channel. */
	virtual void Pause(Channel* channel) = 0;

	/** Stops playback or recording of a channel. */
	virtual void Stop(Channel* channel) = 0;
	//@}

	/** @name Records. */
	//@{
	virtual bool StartRecord(Sound* sound) = 0;

	virtual bool StopRecord() = 0;

	virtual int32 GetRecordPosition() = 0;
	//@}

protected:
	AudioDeviceList _audioDevices;
	AudioSystemDescription _audioSystemDescription;
	int32 _streamBufferSize;
	Listener* _listener;
	ChannelGroup* _masterChannelGroup;
};

SEPointer(AudioSystem);

}
}

#endif 

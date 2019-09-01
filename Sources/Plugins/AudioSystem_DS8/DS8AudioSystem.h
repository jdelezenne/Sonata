/*=============================================================================
DS8AudioSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DS8AUDIOSYSTEM_H_
#define _SE_DS8AUDIOSYSTEM_H_

#include <dsound.h>

#ifdef SE_USE_EAX
// EAX headers
#include <eax.h>
#endif

#include <Core/Core.h>
#include <Audio/Audio.h>

using namespace SonataEngine;
using namespace SonataEngine::Audio;

namespace SE_DS8
{

struct DS8AudioDevice
{
	GUID Guid;
	String strDescription;
	String strModule;
};

/** DirectSound8 Audio System. */
class DS8AudioSystem : public AudioSystem
{
public:
	DS8AudioSystem();
	virtual ~DS8AudioSystem();

	virtual bool EnumerateDevices();

	virtual bool Create(const AudioSystemDescription& desc);

	virtual void Destroy();

	virtual void Update();

	virtual Sound* CreateSoundSample(const AudioFormat& format, SEbyte* buffer, int32 sampleCount, SoundMode mode = SoundMode_Default);

	virtual Sound* CreateSoundStream(const AudioFormat& format, Stream& stream, SoundMode mode = SoundMode_Default);

	virtual void DestroySound(Sound* sound);

	virtual Channel* CreateChannel(Sound* sound);

	virtual void DestroyChannel(Channel* channel);

	virtual void Play(Channel* channel);

	virtual void Pause(Channel* channel);

	virtual void Stop(Channel* channel);

	virtual bool StartRecord(Sound* sound);

	virtual bool StopRecord();

	virtual int32 GetRecordPosition();

	IDirectSound8* GetDS() const { return _pDS; }

private:
	void Init();
	void AddDSDevice(AudioDeviceType type, const DS8AudioDevice& device);

protected:
	bool _CleanupCOM;
	LPDIRECTSOUND8 _pDS;
	LPDIRECTSOUNDCAPTURE _pDSCapture;
	LPDIRECTSOUNDCAPTUREBUFFER _pDSCaptureBuffer;
	LPDIRECTSOUNDBUFFER _pDSBPrimaryBuffer;
	LPDIRECTSOUND3DLISTENER _pDSListener;

	typedef Array<SoundPtr> SoundList;
	SoundList _Sounds;

	typedef Array<ChannelPtr> ChannelList;
	ChannelList _Channels;
};

}

#endif 

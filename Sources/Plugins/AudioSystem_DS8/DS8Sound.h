/*=============================================================================
DS8Sound.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DS8SOUND_H_
#define _SE_DS8SOUND_H_

#include "DS8AudioSystem.h"

namespace SE_DS8
{

/** DirectSound8 Sound. */
class DS8Sound : public Sound
{
public:
	DS8Sound();
	virtual ~DS8Sound();

	bool Create(DS8AudioSystem* audioSystem, const AudioFormat& format, SEbyte* buffer, int32 sampleCount, SoundMode mode);

	virtual int32 GetLength() const { return _Length; }

	SEbyte* GetBuffer() const { return _Buffer; }
	int32 GetSampleCount() const { return _SampleCount; }
	SoundMode GetSoundMode() const { return _Mode; }

protected:
	DS8AudioSystem* _AudioSystem;
	int32 _Length;
	SEbyte* _Buffer;
	int32 _SampleCount;
	SoundMode _Mode;
};

}

#endif 

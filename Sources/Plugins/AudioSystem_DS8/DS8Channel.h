/*=============================================================================
DS8Channel.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DS8CHANNEL_H_
#define _SE_DS8CHANNEL_H_

#include "DS8AudioSystem.h"
#include "DS8Sound.h"

namespace SE_DS8
{

/** DirectSound8 Channel. */
class DS8Channel : public Channel
{
public:
	DS8Channel();
	virtual ~DS8Channel();

	bool Create(DS8AudioSystem* audioSystem, DS8Sound* sound, bool streamed, LPDIRECTSOUNDBUFFER pDSB, LPDIRECTSOUND3DBUFFER pDS3DB, int32 sampleSize, int32 bufferSize);

	virtual void Play();

	virtual void Pause();

	virtual void Stop();

	void Update();

	LPDIRECTSOUNDBUFFER GetDSB() const { return _pDSB; }
	LPDIRECTSOUND3DBUFFER GetDS3DB() const { return _pDS3DB; }

protected:
	DS8AudioSystem* _AudioSystem;
	SmartPtr<DS8Sound> _Sound;

    bool _Streamed;
	LPDIRECTSOUNDBUFFER _pDSB;
	LPDIRECTSOUND3DBUFFER _pDS3DB;
    int32 _SampleSize;
    int32 _bufferSize;
};

}

#endif 

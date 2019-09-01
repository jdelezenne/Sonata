/*=============================================================================
DS8Channel.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DS8Channel.h"

namespace SE_DS8
{

DS8Channel::DS8Channel() :
	Channel(),
	_AudioSystem(NULL),
	_Sound(NULL),
	_Streamed(false),
	_pDSB(NULL),
	_pDS3DB(NULL),
	_SampleSize(0),
	_bufferSize(0)
{
}

DS8Channel::~DS8Channel()
{
	SE_RELEASE(_pDS3DB);
	SE_RELEASE(_pDSB);
}

bool DS8Channel::Create(DS8AudioSystem* audioSystem, DS8Sound* sound, bool streamed, LPDIRECTSOUNDBUFFER pDSB, LPDIRECTSOUND3DBUFFER pDS3DB, int32 sampleSize, int32 bufferSize)
{
	_AudioSystem = audioSystem;
	_Sound = sound;
	_Streamed = streamed;
	_pDSB = pDSB;
	_pDS3DB = pDS3DB;
	_SampleSize = sampleSize;
	_bufferSize = bufferSize;

	return true;
}

void DS8Channel::Play()
{
	if (_Streamed)
	{
	}

	HRESULT hr = _pDSB->Play(0, 0, _Sound->IsLooped() ? DSBPLAY_LOOPING : 0);

	_SoundPlayState = SoundPlayState_Play;
}

void DS8Channel::Pause()
{
	_pDSB->Stop();

	_SoundPlayState = SoundPlayState_Pause;
}

void DS8Channel::Stop()
{
    _pDSB->SetCurrentPosition(0);

	_SoundPlayState = SoundPlayState_Stop;
}

void DS8Channel::Update()
{
	if (_Streamed)
	{
		DWORD dwPlayCursor, dwWriteCursor;
		_pDSB->GetCurrentPosition(&dwPlayCursor,&dwWriteCursor); 
	}

	/*_pDSB->SetFrequency((DWORD)_Frequency);
	_pDSB->SetVolume((DWORD)_Volume);
	_pDSB->SetPan(_Pan);

	_pDS3DB->SetPosition(_Position.x, _Position.y, _Position.z, DS3D_DEFERRED);
	_pDS3DB->SetVelocity(_Velocity.x, _Velocity.y, _Velocity.z, DS3D_DEFERRED);

	DWORD d3dMode;
	if (_Mode == SoundMode3D_Disabled)
		d3dMode = DS3DMODE_DISABLE;
	else if (_Mode == SoundMode3D_HeadRelative)
		d3dMode = DS3DMODE_HEADRELATIVE;
	else
		d3dMode = DS3DMODE_NORMAL;

	_pDS3DB->SetMode(d3dMode, DS3D_DEFERRED);
	_pDS3DB->SetMinDistance(_MinDistance, DS3D_DEFERRED);
	_pDS3DB->SetMaxDistance(_MaxDistance, DS3D_DEFERRED);
	_pDS3DB->SetConeAngles(_InsideConeAngle, _OutsideConeAngle, DS3D_DEFERRED);
	_pDS3DB->SetConeOutsideVolume(_ConeOutsideVolume, DS3D_DEFERRED);
	_pDS3DB->SetConeOrientation(_ConeOrientation.x, _ConeOrientation.y, _ConeOrientation.z, DS3D_DEFERRED);*/
}

}

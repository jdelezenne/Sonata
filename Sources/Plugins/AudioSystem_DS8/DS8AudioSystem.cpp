/*=============================================================================
DS8AudioSystem.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DS8AudioSystem.h"
#include "DS8Sound.h"
#include "DS8Channel.h"

namespace SE_DS8
{

BaseArray<DS8AudioDevice> g_PlaybackDevices;
BaseArray<DS8AudioDevice> g_RecordDevices;

// Callback function that receives DirectSound enumerated devices.
static BOOL CALLBACK DSEnumCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
	AudioDeviceType type = (AudioDeviceType)(int)lpContext;

	DS8AudioDevice device;
	if (lpGuid != NULL)
		Memory::Copy(&device.Guid, lpGuid, sizeof(GUID));
	else
		Memory::Set(&device.Guid, 0, sizeof(GUID));

	device.strDescription = lpcstrDescription;
	device.strModule = lpcstrModule;

	if (type == AudioDeviceType_Playback)
		g_PlaybackDevices.Add(device);
	else if (type == AudioDeviceType_Record)
		g_RecordDevices.Add(device);

	return TRUE;
}

D3DVECTOR MakeD3DVector(const Vector3& value)
{
	D3DVECTOR result;
	result.x = value.X;
	result.y = value.Y;
	result.z = value.Z;
	return result;
}

DS8AudioSystem::DS8AudioSystem() :
	AudioSystem()
{
	Init();

	// Initialize the COM library
	HRESULT hr = ::CoInitialize(NULL);
	_CleanupCOM = SUCCEEDED(hr);

	EnumerateDevices();
}

DS8AudioSystem::~DS8AudioSystem()
{
	Destroy();

	if (_CleanupCOM)
	{
		// Close the COM library
		CoUninitialize();
		_CleanupCOM = false;
	}
}

void DS8AudioSystem::Init()
{
	_CleanupCOM = false;

	_pDS = NULL;
	_pDSCapture = NULL;
	_pDSCaptureBuffer = NULL;
	_pDSBPrimaryBuffer = NULL;
	_pDSListener = NULL;
}

bool DS8AudioSystem::EnumerateDevices()
{
	HRESULT hr;

	_audioDevices.Clear();

	g_PlaybackDevices.Clear();
	g_RecordDevices.Clear();

	// Enumerate playback devices
	hr = DirectSoundEnumerate(DSEnumCallback, (LPVOID)AudioDeviceType_Playback);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.EnumerateDevices"),
			_T("Failed to enumerate the audio playback devices."));
		return false;
	}

	// Enumerate capture devices
	hr = DirectSoundCaptureEnumerate(DSEnumCallback, (LPVOID)AudioDeviceType_Record);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.EnumerateDevices"),
			_T("Failed to enumerate the audio record devices."));
	}

	// Add the devices
	int i;
	int count;

	count = g_PlaybackDevices.Count();
	for (i=0; i<count; i++)
	{
		const DS8AudioDevice device = g_PlaybackDevices[i];
		AddDSDevice(AudioDeviceType_Playback, device);
	}

	count = g_RecordDevices.Count();
	for (i=0; i<count; i++)
	{
		const DS8AudioDevice device = g_RecordDevices[i];
		AddDSDevice(AudioDeviceType_Record, device);
	}

	return true;
}

void DS8AudioSystem::AddDSDevice(AudioDeviceType type, const DS8AudioDevice& device)
{
	HRESULT hr;
	LPDIRECTSOUND8 pDS = NULL;
	LPDIRECTSOUNDCAPTURE pDSCapture = NULL;

	if (type == AudioDeviceType_Playback)
	{
		hr = DirectSoundCreate8(&device.Guid, &pDS, NULL);
	}
	else// if (type == AudioDeviceType_Record)
	{
		hr = DirectSoundCaptureCreate8(&device.Guid, &pDSCapture, NULL);
	}
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.EnumerateDevices"),
			_T("Failed to create a DirectSound8 object."));
		return;
	}

	SpeakerMode speakerMode = SpeakerMode_Stereo;
	if (type == AudioDeviceType_Playback)
	{
		DSCAPS caps;
		DWORD dwSpeakerConfig;

		pDS->GetCaps(&caps);
		pDS->GetSpeakerConfig(&dwSpeakerConfig);
		pDS->Release();

		switch (dwSpeakerConfig)
		{
		case DSSPEAKER_DIRECTOUT: speakerMode = SpeakerMode_DirectOut; break;
		case DSSPEAKER_HEADPHONE: speakerMode = SpeakerMode_Headphone; break;
		case DSSPEAKER_MONO: speakerMode = SpeakerMode_Mono; break;
		case DSSPEAKER_STEREO: speakerMode = SpeakerMode_Stereo; break;
		case DSSPEAKER_QUAD: speakerMode = SpeakerMode_4Point1; break;
		case DSSPEAKER_5POINT1: speakerMode = SpeakerMode_5Point1; break;
		case DSSPEAKER_7POINT1: speakerMode = SpeakerMode_7Point1; break;
		case DSSPEAKER_SURROUND: speakerMode = SpeakerMode_Surround; break;
		}

		AudioDeviceCaps audioDeviceCaps(caps.dwMinSecondarySampleRate,
			caps.dwMaxSecondarySampleRate, speakerMode);

		AudioDevice audioDevice(_audioDevices.Count(), type, device.strDescription, audioDeviceCaps);

		_audioDevices.Add(audioDevice);
	}
	else// if (type == AudioDeviceType_Record)
	{
		DSCCAPS ccaps;

		pDSCapture->GetCaps(&ccaps);
		pDSCapture->Release();

		AudioDeviceCaps audioDeviceCaps(0, 0, SpeakerMode_DirectOut);

		AudioDevice audioDevice(_audioDevices.Count(), type, device.strDescription, audioDeviceCaps);

		_audioDevices.Add(audioDevice);
	}
}

bool DS8AudioSystem::Create(const AudioSystemDescription& desc)
{
	HRESULT hr;

	LPGUID lpGUID;
	if (desc.UsePlayback)
	{
		if (desc.PlaybackDevice < 0 || desc.PlaybackDevice >= g_PlaybackDevices.Count())
			lpGUID = NULL; // Default device
		else
			lpGUID = &g_PlaybackDevices[desc.PlaybackDevice].Guid;

		// Create a DirectSound object 
		if (!desc.UseEAX)
		{
			hr = DirectSoundCreate8(lpGUID, &_pDS, NULL);
		}
		else
		{
#ifdef SE_USE_EAX
			hr = EAXDirectSoundCreate8(lpGUID, &_pDS, NULL);
#else
			hr = DirectSoundCreate8(lpGUID, &_pDS, NULL);
#endif
		}

		if (FAILED(hr))
		{
			Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.Create (DirectSoundCreate8)"),
				_T("Failed to create a DirectSound8 object."));
			return false;
		}
	}

	if (desc.UseRecord)
	{
		if (desc.RecordDevice < 0 || desc.RecordDevice >= g_RecordDevices.Count())
			lpGUID = NULL; // Default capture device
		else
			lpGUID = &g_RecordDevices[desc.RecordDevice].Guid;

		// Create a DirectSoundCapture object 
		hr = DirectSoundCaptureCreate8(lpGUID, &_pDSCapture, NULL);
		if (FAILED(hr))
		{
			Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.Create (DirectSoundCreate8)"),
				_T("Failed to create a DirectSound8 capture object."));
			return false;
		}
	}

	// Cooperative level
	HWND hWnd;
	if (desc.Window != NULL)
	{
		hWnd = (HWND)desc.Window->GetHandle();
	}
	else
	{
		// A window is required (GetDesktopWindow doesn't work)
		WNDCLASS wndClass;

		wndClass.style = 0;
		wndClass.lpfnWndProc = DefWindowProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = ::GetModuleHandle(NULL);
		wndClass.hIcon = NULL;
		wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "DS8Dummy";

		::RegisterClass(&wndClass);

		hWnd = ::CreateWindow(
			"DS8Dummy",
			"DS8Dummy",
			0,
			0,
			0,
			1,
			1,
			NULL,
			NULL,
			::GetModuleHandle(NULL),
			NULL);
	}

	hr = _pDS->SetCooperativeLevel(hWnd, desc.ExclusiveLevel ? DSSCL_EXCLUSIVE : DSSCL_NORMAL);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.Create (SetCooperativeLevel)"),
			_T("Failed to set the cooperative level."));
		return false;
	}

	// Create the primary buffer 
	DSBUFFERDESC dsbd;
	::ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	hr = _pDS->CreateSoundBuffer(&dsbd, &_pDSBPrimaryBuffer, NULL);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.Create (CreateSoundBuffer)"),
			_T("Failed to create the primary sound buffer."));
		return false;
	}

	if (!desc.DefaultFormat)
	{
		WAVEFORMATEX wfx;
		::ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
		wfx.wFormatTag = WAVE_FORMAT_PCM;
		wfx.nChannels = desc.Format.Channels;
		wfx.nSamplesPerSec = desc.Format.SamplesPerSecond;
		wfx.nAvgBytesPerSec = desc.Format.GetAverageBytesPerSecond();
		wfx.nBlockAlign = desc.Format.GetBlockAlign();
		wfx.wBitsPerSample = desc.Format.GetBitsPerSample();
		wfx.cbSize = sizeof(WAVEFORMATEX);

		hr = _pDSBPrimaryBuffer->SetFormat(&wfx);
		if (FAILED(hr))
		{
			Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.Create"),
				_T("Failed to set the primary buffer format."));
			return false;
		}
	}

	hr = _pDSBPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener, (VOID**)&_pDSListener);
	if (FAILED(hr))
	{
		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.Create"),
			_T("Failed to retrieve the listener."));
		return false;
	}

	_audioSystemDescription = desc;

	return true;
}

void DS8AudioSystem::Destroy()
{
	SE_RELEASE(_pDSListener);
	SE_RELEASE(_pDSBPrimaryBuffer);
	SE_RELEASE(_pDSCaptureBuffer);
	SE_RELEASE(_pDSCapture);

	// Release DirectSound object
	SE_RELEASE(_pDS);
}

void DS8AudioSystem::Update()
{
	ChannelList::Iterator it = _Channels.GetIterator();
	while (it.Next())
	{
		((DS8Channel*)it.Current().Get())->Update();
	}

	if (_listener != NULL)
	{
		DS3DLISTENER dsListener;
		dsListener.dwSize = sizeof(DS3DLISTENER);
		dsListener.vPosition = MakeD3DVector(_listener->GetPosition());
		dsListener.vVelocity = MakeD3DVector(_listener->GetVelocity());
		dsListener.vOrientFront = MakeD3DVector(_listener->GetOrientationFront());
		dsListener.vOrientTop = MakeD3DVector(_listener->GetOrientationTop());
		dsListener.flDistanceFactor = _listener->GetDistanceFactor();
		dsListener.flRolloffFactor = _listener->GetRolloffFactor();
		dsListener.flDopplerFactor = _listener->GetDopplerFactor();

		_pDSListener->SetAllParameters(&dsListener, DS3D_IMMEDIATE);
	}
}

Sound* DS8AudioSystem::CreateSoundSample(const AudioFormat& format, SEbyte* buffer, int32 sampleCount, SoundMode mode)
{
	if (format.FormatType != AudioFormatType_PCM8 && format.FormatType != AudioFormatType_PCM16 &&
		format.FormatType != AudioFormatType_PCM24 && format.FormatType != AudioFormatType_PCM32)
	{
		return NULL;
	}

	if (mode == SoundMode_Default)
		mode = (SoundMode)(SoundMode_2D | SoundMode_Hardware);

	DS8Sound* sound = new DS8Sound();
	sound->Create(this, format, buffer, sampleCount, mode);

	// Add it to the list of sounds
	_Sounds.Add(sound);

	return sound;
}

Sound* DS8AudioSystem::CreateSoundStream(const AudioFormat& format, Stream& stream, SoundMode mode)
{
	return NULL;
}

void DS8AudioSystem::DestroySound(Sound* sound)
{
	_Sounds.Remove(sound);
}

Channel* DS8AudioSystem::CreateChannel(Sound* sound)
{
	DS8Sound* ds8Sound = (DS8Sound*)sound;

	HRESULT hr;
	AudioFormat format = ds8Sound->GetFormat();
	SEbyte* buffer = ds8Sound->GetBuffer();
	int32 sampleCount = ds8Sound->GetSampleCount();
	SoundMode mode = ds8Sound->GetSoundMode();

    int32 sampleSize = format.GetBlockAlign();

	WAVEFORMATEX wfx;
	::ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = format.Channels;
	wfx.nSamplesPerSec = format.SamplesPerSecond;
	wfx.nAvgBytesPerSec = format.GetAverageBytesPerSecond();
	wfx.nBlockAlign = sampleSize;
	wfx.wBitsPerSample = format.GetBitsPerSample();
	wfx.cbSize = sizeof(WAVEFORMATEX);

	DSBUFFERDESC dsbd;
	::ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;

	if ((mode & SoundMode_3D) != 0)
		dsbd.dwFlags |= DSBCAPS_CTRL3D;

	if ((mode & SoundMode_Global) != 0)
		dsbd.dwFlags |= DSBCAPS_GLOBALFOCUS;

	if ((mode & SoundMode_ForceHardware) != 0)
		dsbd.dwFlags |= DSBCAPS_LOCHARDWARE;

	if ((mode & SoundMode_Software) != 0)
		dsbd.dwFlags |= DSBCAPS_LOCSOFTWARE;

    int32 bufferSize = sampleCount * sampleSize;
	dsbd.dwBufferBytes = bufferSize;
	dsbd.lpwfxFormat = &wfx;

	// Create the DirectSound buffer
	LPDIRECTSOUNDBUFFER pDSB;
	hr = _pDS->CreateSoundBuffer(&dsbd, &pDSB, NULL);
	if (FAILED(hr) || pDSB == NULL)
	{
		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.CreateChannel"),
			_T("Failed to create the DirectSound buffer."));
		return false;
	}

	// Fill the sound buffer
    void* data;
    DWORD dataSize;
	hr = pDSB->Lock(0, bufferSize, &data, &dataSize, NULL, NULL, 0);
	if (FAILED(hr))
	{
		pDSB->Release();

		Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.CreateChannel"),
			_T("Failed to lock the DirectSound buffer."));
		return false;
	}

	::CopyMemory(data, buffer, dataSize);

	pDSB->Unlock(data, dataSize, 0, 0);

	LPDIRECTSOUND3DBUFFER pDS3DB = NULL;
	if ((mode & SoundMode_3D) != 0)
	{
		hr = pDSB->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID*)&pDS3DB);
		if (FAILED(hr))
		{
			Logger::Current()->Log(LogLevel::Error, _T("DS8AudioSystem.CreateChannel"),
				_T("Failed to get the DirectSound 3D buffer."));
		}
	}

	DS8Channel* channel = new DS8Channel();
	channel->Create(this, ds8Sound, false, pDSB, pDS3DB, sampleSize, bufferSize);

	return channel;
}

void DS8AudioSystem::DestroyChannel(Channel* channel)
{
	_Channels.Remove(channel);
}

void DS8AudioSystem::Play(Channel* channel)
{
	if (channel == NULL)
		return;

	channel->Play();
}

void DS8AudioSystem::Pause(Channel* channel)
{
	if (channel == NULL)
		return;

	channel->Pause();
}

void DS8AudioSystem::Stop(Channel* channel)
{
	if (channel == NULL)
		return;

	channel->Stop();
}

bool DS8AudioSystem::StartRecord(Sound* sound)
{
	return false;
}

bool DS8AudioSystem::StopRecord()
{
	return false;
}

int32 DS8AudioSystem::GetRecordPosition()
{
	return 0;
}

}

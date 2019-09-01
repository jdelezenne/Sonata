/*=============================================================================
Win32Helper.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Helper.h"

namespace SonataEngine
{

HWND Win32Helper::MakeHandle(WindowHandle handle)
{
	return (HWND)handle;
}

WindowHandle Win32Helper::MakeHandle(HWND handle)
{
	return (WindowHandle)handle;
}
/*
POINT Win32Helper::MakePoint(Point point)
{
	POINT ptNew = { point.x, point.y };
	return ptNew;
}

Point Win32Helper::MakePoint(POINT point)
{
	return Point(point.x, point.y);
}

SIZE Win32Helper::MakeSize(Size size)
{
	SIZE szNew = { size.width, size.height };
	return szNew;
}

Size Win32Helper::MakeSize(SIZE size)
{
	return Size(size.cx, size.cy);
}
*/
RECT Win32Helper::MakeRect(const RectangleInt& rect)
{
	RECT rcNew = { rect.X, rect.Y, rect.Width, rect.Height };
	return rcNew;
}

RectangleInt Win32Helper::MakeRect(RECT rect)
{
	return RectangleInt(rect.left, rect.top, rect.right, rect.bottom);
}

/*WAVEFORMATEX Win32Helper::MakeWaveFormat(WaveFormat waveFormat)
{
	WAVEFORMATEX newWaveFormat;
	newWaveFormat.wFormatTag		= waveFormat.wFormatTag;
	newWaveFormat.nChannels			= waveFormat.nChannels;
	newWaveFormat.nSamplesPerSec	= waveFormat.nSamplesPerSec;
	newWaveFormat.nAvgBytesPerSec	= waveFormat.nAvgBytesPerSec;
	newWaveFormat.nBlockAlign		= waveFormat.nBlockAlign;
	newWaveFormat.wBitsPerSample	= waveFormat.wBitsPerSample;
	newWaveFormat.cbSize			= 0;
	return newWaveFormat;
}

WaveFormat Win32Helper::MakeWaveFormat(WAVEFORMATEX waveFormat)
{
	WaveFormat newWaveFormat;
	newWaveFormat.wFormatTag		= waveFormat.wFormatTag;
	newWaveFormat.nChannels			= waveFormat.nChannels;
	newWaveFormat.nSamplesPerSec	= waveFormat.nSamplesPerSec;
	newWaveFormat.nAvgBytesPerSec	= waveFormat.nAvgBytesPerSec;
	newWaveFormat.nBlockAlign		= waveFormat.nBlockAlign;
	newWaveFormat.wBitsPerSample	= waveFormat.wBitsPerSample;
	return newWaveFormat;
}

#ifdef USE_EAX
EAXLISTENERPROPERTIES Win32Helper::MakeEAXListener(EAXListenerProperties EAXlistener)
{
	EAXLISTENERPROPERTIES newEAXlistener;
	newEAXlistener.lRoom = EAXlistener.lRoom;
	newEAXlistener.lRoomHF = EAXlistener.lRoomHF; 
	newEAXlistener.flRoomRolloffFactor = EAXlistener.fRoomRolloffFactor; 
	newEAXlistener.flDecayTime = EAXlistener.fDecayTime; 
	newEAXlistener.flDecayHFRatio = EAXlistener.fDecayHFRatio; 
	newEAXlistener.lReflections = EAXlistener.lReflections; 
	newEAXlistener.flReflectionsDelay = EAXlistener.fReflectionsDelay; 
	newEAXlistener.lReverb = EAXlistener.lReverb;	
	newEAXlistener.flReverbDelay = EAXlistener.fReverbDelay;	
	newEAXlistener.dwEnvironment = EAXlistener.dwEnvironment;	
	newEAXlistener.flEnvironmentSize = EAXlistener.fEnvironmentSize;	
	newEAXlistener.flEnvironmentDiffusion = EAXlistener.fEnvironmentDiffusion;	
	newEAXlistener.flAirAbsorptionHF = EAXlistener.fAirAbsorptionHF;	

	newEAXlistener.dwFlags = 0;
	newEAXlistener.dwFlags |= EAXlistener.bScaleDecayTime ? EAXLISTENERFLAGS_DECAYTIMESCALE : 0;
	newEAXlistener.dwFlags |= EAXlistener.bClipDecayHF ? EAXLISTENERFLAGS_DECAYHFLIMIT : 0;
	newEAXlistener.dwFlags |= EAXlistener.bScaleReflections ? EAXLISTENERFLAGS_REFLECTIONSSCALE : 0;
	newEAXlistener.dwFlags |= EAXlistener.bScaleReflectionsDelay ? EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE : 0;
	newEAXlistener.dwFlags |= EAXlistener.bScaleReverb ? EAXLISTENERFLAGS_REVERBSCALE : 0;
	newEAXlistener.dwFlags |= EAXlistener.bScaleReverbDelay ? EAXLISTENERFLAGS_REVERBDELAYSCALE : 0;
	return newEAXlistener;
}

EAXListenerProperties Win32Helper::MakeEAXListener(EAXLISTENERPROPERTIES EAXlistener)
{
	EAXListenerProperties newEAXlistener;
	newEAXlistener.lRoom = EAXlistener.lRoom;
	newEAXlistener.lRoomHF = EAXlistener.lRoomHF; 
	newEAXlistener.fRoomRolloffFactor = EAXlistener.flRoomRolloffFactor; 
	newEAXlistener.fDecayTime = EAXlistener.flDecayTime; 
	newEAXlistener.fDecayHFRatio = EAXlistener.flDecayHFRatio; 
	newEAXlistener.lReflections = EAXlistener.lReflections; 
	newEAXlistener.fReflectionsDelay = EAXlistener.flReflectionsDelay; 
	newEAXlistener.lReverb = EAXlistener.lReverb;	
	newEAXlistener.fReverbDelay = EAXlistener.flReverbDelay;	
	newEAXlistener.dwEnvironment = EAXlistener.dwEnvironment;	
	newEAXlistener.fEnvironmentSize = EAXlistener.flEnvironmentSize;	
	newEAXlistener.fEnvironmentDiffusion = EAXlistener.flEnvironmentDiffusion;	
	newEAXlistener.fAirAbsorptionHF = EAXlistener.flAirAbsorptionHF;	
	newEAXlistener.bScaleDecayTime = (EAXlistener.dwFlags & EAXLISTENERFLAGS_DECAYTIMESCALE) != 0;

	newEAXlistener.bClipDecayHF = (EAXlistener.dwFlags & EAXLISTENERFLAGS_DECAYHFLIMIT) != 0;
	newEAXlistener.bScaleReflections = (EAXlistener.dwFlags & EAXLISTENERFLAGS_REFLECTIONSSCALE) != 0;
	newEAXlistener.bScaleReflectionsDelay = (EAXlistener.dwFlags & EAXLISTENERFLAGS_REFLECTIONSDELAYSCALE) != 0;
	newEAXlistener.bScaleReverb = (EAXlistener.dwFlags & EAXLISTENERFLAGS_REVERBSCALE) != 0;
	newEAXlistener.bScaleReverbDelay = (EAXlistener.dwFlags & EAXLISTENERFLAGS_REVERBDELAYSCALE) != 0;
	return newEAXlistener;
}

EAXBUFFERPROPERTIES Win32Helper::MakeEAXBuffer(EAXBufferProperties EAXbuffer)
{
	EAXBUFFERPROPERTIES newEAXbuffer;
	newEAXbuffer.lDirect = EAXbuffer.lDirect;
	newEAXbuffer.lDirectHF = EAXbuffer.lDirectHF; 
	newEAXbuffer.lRoom = EAXbuffer.lRoom; 
	newEAXbuffer.lRoomHF = EAXbuffer.lRoomHF; 
	newEAXbuffer.flRoomRolloffFactor = EAXbuffer.fRoomRolloffFactor; 
	newEAXbuffer.lObstruction = EAXbuffer.lObstruction; 
	newEAXbuffer.flObstructionLFRatio = EAXbuffer.fObstructionLFRatio;	
	newEAXbuffer.lOcclusion = EAXbuffer.lOcclusion;	
	newEAXbuffer.flOcclusionLFRatio = EAXbuffer.fOcclusionLFRatio;	
	newEAXbuffer.flOcclusionRoomRatio = EAXbuffer.fOcclusionRoomRatio;	
	newEAXbuffer.lOutsideVolumeHF = EAXbuffer.lOutsideVolumeHF;	
	newEAXbuffer.flAirAbsorptionFactor = EAXbuffer.fAirAbsorptionFactor;	

	newEAXbuffer.dwFlags = 0;
	newEAXbuffer.dwFlags |= EAXbuffer.bDirectHFAuto ? EAXBUFFERFLAGS_DIRECTHFAUTO : 0;
	newEAXbuffer.dwFlags |= EAXbuffer.bRoomAuto ? EAXBUFFERFLAGS_ROOMAUTO : 0;
	newEAXbuffer.dwFlags |= EAXbuffer.bRoomHFAuto ? EAXBUFFERFLAGS_ROOMHFAUTO : 0;
	return newEAXbuffer;
}

EAXBufferProperties Win32Helper::MakeEAXBuffer(EAXBUFFERPROPERTIES EAXbuffer)
{
	EAXBufferProperties newEAXbuffer;
	newEAXbuffer.lDirect = EAXbuffer.lDirect;
	newEAXbuffer.lDirectHF = EAXbuffer.lDirectHF; 
	newEAXbuffer.lRoom = EAXbuffer.lRoom; 
	newEAXbuffer.lRoomHF = EAXbuffer.lRoomHF; 
	newEAXbuffer.fRoomRolloffFactor = EAXbuffer.flRoomRolloffFactor; 
	newEAXbuffer.lObstruction = EAXbuffer.lObstruction; 
	newEAXbuffer.fObstructionLFRatio = EAXbuffer.flObstructionLFRatio;	
	newEAXbuffer.lOcclusion = EAXbuffer.lOcclusion;	
	newEAXbuffer.fOcclusionLFRatio = EAXbuffer.flOcclusionLFRatio;	
	newEAXbuffer.fOcclusionRoomRatio = EAXbuffer.flOcclusionRoomRatio;	
	newEAXbuffer.lOutsideVolumeHF = EAXbuffer.lOutsideVolumeHF;	
	newEAXbuffer.fAirAbsorptionFactor = EAXbuffer.flAirAbsorptionFactor;	

	newEAXbuffer.bDirectHFAuto = (EAXbuffer.dwFlags & EAXBUFFERFLAGS_DIRECTHFAUTO) != 0;
	newEAXbuffer.bRoomAuto = (EAXbuffer.dwFlags & EAXBUFFERFLAGS_ROOMAUTO) != 0;
	newEAXbuffer.bRoomHFAuto = (EAXbuffer.dwFlags & EAXBUFFERFLAGS_ROOMHFAUTO) != 0;
	return newEAXbuffer;
}
#endif*/

}

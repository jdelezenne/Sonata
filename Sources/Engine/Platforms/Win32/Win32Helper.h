/*=============================================================================
Win32Helper.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_WIN32HELPER_H_
#define _SE_WIN32HELPER_H_

#include "Win32Platform.h"
#include "Core/Common.h"
#include "Core/Rectangle.h"
#include "Core/System/Window.h"

namespace SonataEngine
{

/** Helper class for the Windows32 systems.
*/
class SE_CORE_EXPORT Win32Helper
{
public:
	// Windows handle.
	static HWND MakeHandle(WindowHandle handle);
	static WindowHandle MakeHandle(HWND handle);

	// Point.
	//static POINT MakePoint(Point point);
	//static Point MakePoint(POINT point);

	// Size.
	//static SIZE MakeSize(Size size);
	//static Size MakeSize(SIZE size);

	// Rectangle.
	static RECT MakeRect(const RectangleInt& rect);
	static RectangleInt MakeRect(RECT rect);
/*
	// Audio.
	static WAVEFORMATEX MakeWaveFormat(WaveFormat waveFormat);
	static WaveFormat MakeWaveFormat(WAVEFORMATEX waveFormat);

#ifdef USE_EAX
	static EAXLISTENERPROPERTIES MakeEAXListener(EAXListenerProperties EAXlistener);
	static EAXListenerProperties MakeEAXListener(EAXLISTENERPROPERTIES EAXlistener);

	static EAXBUFFERPROPERTIES MakeEAXBuffer(EAXBufferProperties EAXbuffer);
	static EAXBufferProperties MakeEAXBuffer(EAXBUFFERPROPERTIES EAXbuffer);
#endif
*/
};

}

#endif 

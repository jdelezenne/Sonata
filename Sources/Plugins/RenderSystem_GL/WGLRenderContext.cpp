/*=============================================================================
WGLRenderContext.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "WGLRenderContext.h"

namespace SE_GL
{

WGLRenderContext::WGLRenderContext() :
	GLRenderContext(),
	_Valid(false),
	_hWnd(NULL),
	_hDC(NULL),
	_hGLRC(NULL)
{
}

WGLRenderContext::~WGLRenderContext()
{
	Destroy();
}

bool WGLRenderContext::IsValid() const
{
	return _Valid;
}

bool WGLRenderContext::Create(Window* window, const RenderContextDescription& desc)
{
	if (window == NULL)
	{
		return false;
	}

	if (_Valid)
	{
		if (!Destroy())
			return false;
	}

	_hWnd = (HWND)window->GetHandle();
	if (_hWnd == NULL)
	{
		Logger::Current()->Log(LogLevel::Warning, _T("WGLRenderContext.Create"),
			_T("The specified window is not valid."));
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	Memory::Copy(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 15;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;

	_hDC = ::GetDC(_hWnd);
	if (_hDC == NULL)
	{
		Logger::Current()->Log(LogLevel::Warning, _T("WGLRenderContext.Create"),
			_T("Failed to retrieve a handle to a display device context of the window."));
		return false;
	}

	//TODO: wglChoosePixelFormatARB

	// Attempts to match an appropriate pixel format supported by
	// a device context to a given pixel format specification
	int nPixelFormat = ChoosePixelFormat(_hDC, &pfd);
	if (nPixelFormat == 0)
	{
		Logger::Current()->Log(LogLevel::Warning, _T("WGLRenderContext.Create (ChoosePixelFormat)"),
			_T("Failed to choose a pixel format."));
		return false;
	}

	SetPixelFormat(_hDC, nPixelFormat, &pfd);

	// Creates an OpenGL rendering context
	_hGLRC = wglCreateContext(_hDC);
	if (_hGLRC == NULL)
	{
		Logger::Current()->Log(LogLevel::Warning, _T("WGLRenderContext.Create (wglCreateContext)"),
			_T("Failed to create an OpenGL rendering context."));
		return false;
	}

	// Makes a specified OpenGL rendering context the calling thread's current rendering context
	BOOL res = wglMakeCurrent(_hDC, _hGLRC);
	if (!res)
	{
		Logger::Current()->Log(LogLevel::Warning, _T("WGLRenderContext.Create (wglMakeCurrent)"),
			_T("Failed to make the OpenGL rendering context current."));
		return false;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		Logger::Current()->Log(LogLevel::Error, _T("WGLRenderContext.Create (glewInit)"),
			_T("Failed to initialize GLEW: " + String((SEchar*)glewGetErrorString(err))));
	}

	//TODO: multisampling
	/*
	if (desc.MultiSamplingType > 0)
	{
		glEnable(GL_MULTISAMPLE_ARB);
	}
	desc.MultiSampleQuality;
	desc.EnableDepthStencil;
	desc.VSync;
	*/

	if (desc.FullScreen)
	{
		SystemDisplay::SetCurrentDisplayMode(desc.Mode, true);
	}

	_Window = window;
	_Description = desc;
	_Valid = true;

	return true;
}

bool WGLRenderContext::Destroy()
{
	if (!_Valid)
		return false;

	if (_Description.FullScreen)
	{
		SystemDisplay::SetCurrentDisplayMode(_Description.Mode, false);
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(_hGLRC);
	ReleaseDC(_hWnd, _hDC);

	_hWnd = NULL;
	_hDC = NULL;
	_hGLRC = NULL;

	_Window = NULL;
	_Valid = false;

	return true;
}

void WGLRenderContext::SwapBuffers()
{
	if (_hDC != NULL)
	{
		::SwapBuffers(_hDC);
	}
}

}

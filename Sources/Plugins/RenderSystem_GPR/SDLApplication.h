/*=============================================================================
SDLApplication.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _SDLAPPLICATION_H_
#define _SDLAPPLICATION_H_

#include <SDL.h>
#include "Core/Application.h"
using namespace Teesside;

/** SDL application. */
class SDLApplication : public Application
{
public:
	/** Constructors / Destructor. */
	//@{
	SDLApplication();
	virtual ~SDLApplication();
	//@}

	virtual bool IsInitialized() const { return _Init; }

	virtual uint32 GetWidth() const { return _Width; }
	virtual void SetWidth(uint32 width) { _Width = width; }

	virtual uint32 GetHeight() const { return _Height; }
	virtual void SetHeight(uint32 height) { _Height = height; }

	virtual void SetWindowSize(uint32 width, uint32 height);

	virtual bool GetFullScreen() const;
	virtual void SetFullScreen(bool fullScreen);

	virtual String GetWindowTitle() const;
	virtual void SetWindowTitle(const String& title);

	virtual real64 GetTime() const;
	virtual real64 GetElapsedTime() const;

	virtual void SetCursorVisible(bool visible);
	virtual void SetCursorLocked(bool locked);
	virtual bool IsKeyDown(KeyCode key) const;
	virtual bool IsKeyUp(KeyCode key) const;
	virtual bool IsButtonDown(MouseButton button) const;
	virtual bool IsButtonUp(MouseButton button) const;

	virtual bool Create();
	virtual bool Destroy();
	virtual bool Run();
	virtual void Exit();
	void Render();

protected:
	void _ProcessEvents();
	void _VideoResizeEvent(const SDL_ResizeEvent& event);
	void _KeyboardEvent(const SDL_KeyboardEvent& event);
	void _MouseMotionEvent(const SDL_MouseMotionEvent& event);
	void _MouseButtonEvent(const SDL_MouseButtonEvent& event);
	void _Update();

	bool _Init;
	bool _Exit;
	uint32 _Width;
	uint32 _Height;
	bool _FullScreen;
	String _Title;
	bool _CursorLocked;

	SDL_Surface* _Screen;
	Uint8* _KeyState;
	Uint8 _MouseState;
};

#endif 

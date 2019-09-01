/*=============================================================================
SDLApplication.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "SDLApplication.h"
#include "Software/SoftwareRenderer.h"
#include "Core/Core.h"

real64 _Time = 0.0;
real64 _ElapsedTime = 0.0;

KeyCode SDLKeyCode(int key)
{
	KeyCode code;

	switch (key)
	{
	case SDLK_BACKSPACE: code = KC_Backspace; break;
	case SDLK_TAB: code = KC_Tab; break;
	case SDLK_CLEAR: code = KC_Clear; break;
	case SDLK_RETURN: code = KC_Return; break;
	case SDLK_PAUSE: code = KC_Pause; break;
	case SDLK_ESCAPE: code = KC_Escape; break;
	case SDLK_SPACE: code = KC_Space; break;
	case SDLK_DELETE: code = KC_Delete; break;
	case SDLK_UP: code = KC_Up; break;
	case SDLK_DOWN: code = KC_Down; break;
	case SDLK_RIGHT: code = KC_Right; break;
	case SDLK_LEFT: code = KC_Left; break;
	case SDLK_INSERT: code = KC_Insert; break;
	case SDLK_HOME: code = KC_Home; break;
	case SDLK_END: code = KC_End; break;
	case SDLK_PAGEUP: code = KC_PageUp; break;
	case SDLK_PAGEDOWN: code = KC_PageDown; break;
	case SDLK_KP_DIVIDE: code = KC_Divide; break;
	case SDLK_KP_MULTIPLY: code = KC_Multiply; break;
	case SDLK_KP_MINUS: code = KC_Subtract; break;
	case SDLK_KP_PLUS: code = KC_Add; break;
	case SDLK_F1: code = KC_F1; break;
	case SDLK_F2: code = KC_F2; break;
	case SDLK_F3: code = KC_F3; break;
	case SDLK_F4: code = KC_F4; break;
	case SDLK_F5: code = KC_F5; break;
	case SDLK_F6: code = KC_F6; break;
	case SDLK_F7: code = KC_F7; break;
	case SDLK_F8: code = KC_F8; break;
	case SDLK_F9: code = KC_F9; break;
	case SDLK_F10: code = KC_F10; break;
	case SDLK_F11: code = KC_F11; break;
	case SDLK_F12: code = KC_F12; break;
	case SDLK_0: code = KC_0; break;
	case SDLK_1: code = KC_1; break;
	case SDLK_2: code = KC_2; break;
	case SDLK_3: code = KC_3; break;
	case SDLK_4: code = KC_4; break;
	case SDLK_5: code = KC_5; break;
	case SDLK_6: code = KC_6; break;
	case SDLK_7: code = KC_7; break;
	case SDLK_8: code = KC_8; break;
	case SDLK_9: code = KC_9; break;
	case SDLK_a: code = KC_A; break;
	case SDLK_b: code = KC_B; break;
	case SDLK_c: code = KC_C; break;
	case SDLK_d: code = KC_D; break;
	case SDLK_e: code = KC_E; break;
	case SDLK_f: code = KC_F; break;
	case SDLK_g: code = KC_G; break;
	case SDLK_h: code = KC_H; break;
	case SDLK_i: code = KC_I; break;
	case SDLK_j: code = KC_J; break;
	case SDLK_k: code = KC_K; break;
	case SDLK_l: code = KC_L; break;
	case SDLK_m: code = KC_M; break;
	case SDLK_n: code = KC_N; break;
	case SDLK_o: code = KC_O; break;
	case SDLK_p: code = KC_P; break;
	case SDLK_q: code = KC_Q; break;
	case SDLK_r: code = KC_R; break;
	case SDLK_s: code = KC_S; break;
	case SDLK_t: code = KC_T; break;
	case SDLK_u: code = KC_U; break;
	case SDLK_v: code = KC_V; break;
	case SDLK_w: code = KC_W; break;
	case SDLK_x: code = KC_X; break;
	case SDLK_y: code = KC_Y; break;
	case SDLK_z: code = KC_Z; break;
	default: code = (KeyCode) key;
	}

	return code;
}

SDLApplication::SDLApplication()
{
	_Init = false;
	_Exit = false;
	_Width = 800;
	_Height = 600;
	_FullScreen = false;
	_CursorLocked = false;

	_Screen = NULL;
	_KeyState = NULL;
	_MouseState = 0;
}

SDLApplication::~SDLApplication()
{
	Destroy();
}

void SDLApplication::SetWindowSize(uint32 width, uint32 height)
{
	_Width = width;
	_Height = height;
}

bool SDLApplication::GetFullScreen() const
{
	return _FullScreen;
}

void SDLApplication::SetFullScreen(bool fullScreen)
{
	_FullScreen = fullScreen;
}

String SDLApplication::GetWindowTitle() const
{
	return _Title;
}

void SDLApplication::SetWindowTitle(const String& title)
{
	_Title = title;
	SDL_WM_SetCaption(_Title, NULL);
}

real64 SDLApplication::GetTime() const
{
	real64 _LastTime = _Time;
	_Time = SDL_GetTicks() * 0.001;
	_ElapsedTime = _Time - _LastTime;
	return _Time;
}

real64 SDLApplication::GetElapsedTime() const
{
	return _ElapsedTime;
}

void SDLApplication::SetCursorVisible(bool visible)
{
	SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
}

void SDLApplication::SetCursorLocked(bool locked)
{
	_CursorLocked = locked;
}

bool SDLApplication::IsKeyDown(KeyCode key) const
{
	if (_KeyState == NULL)
		return false;

	for (int i = 0; i < 256; i++)
	{
		KeyCode sdlKey = SDLKeyCode(i);
		if (key == sdlKey)
			return _KeyState[i] != 0;
	}

	return false;
}

bool SDLApplication::IsKeyUp(KeyCode key) const
{
	if (_KeyState == NULL)
		return false;

	for (int i = 0; i < 256; i++)
	{
		KeyCode sdlKey = SDLKeyCode(i);
		if (key == sdlKey)
			return _KeyState[i] == 0;
	}

	return true;
}

bool SDLApplication::IsButtonDown(MouseButton button) const
{
	Uint8 sdlButton;
	if (button == MB_Left) sdlButton = SDL_BUTTON(1);
	else if (button == MB_Right) sdlButton = SDL_BUTTON(3);
	else if (button == MB_Middle) sdlButton = SDL_BUTTON(2);

	return (_MouseState & sdlButton) != 0;
}

bool SDLApplication::IsButtonUp(MouseButton button) const
{
	Uint8 sdlButton;
	if (button == MB_Left) sdlButton = SDL_BUTTON(1);
	else if (button == MB_Right) sdlButton = SDL_BUTTON(3);
	else if (button == MB_Middle) sdlButton = SDL_BUTTON(2);

	return (_MouseState & sdlButton) == 0;
}

bool SDLApplication::Create()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
		return false;

	atexit(SDL_Quit);

	const SDL_VideoInfo* info = SDL_GetVideoInfo();

	int bpp = info->vfmt->BitsPerPixel;
	int flags = SDL_SWSURFACE;

	if (_FullScreen)
		flags |= SDL_FULLSCREEN;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_Screen = SDL_SetVideoMode(_Width, _Height, bpp, flags);

	SDL_WM_SetCaption(_Title, NULL);

	Renderer* renderer = new SoftwareRenderer();
	Core::Instance()->SetRenderer(renderer);
	renderer->Create(_Width, _Height);

	_Init = true;

	if (!OnCreate())
	{
		Destroy();
		return false;
	}

	return true;
}

bool SDLApplication::Destroy()
{
	if (!_Init)
		return false;

	OnDestroy();

	if (_Screen != NULL)
	{
		SDL_FreeSurface(_Screen);
		_Screen = NULL;
	}

	Core::Instance()->Destroy();
	_Init = false;

	return true;
}

bool SDLApplication::Run()
{
	if (!_Init)
		return false;

	_ProcessEvents();

	return true;
}

void SDLApplication::Exit()
{
	Destroy();

	_Exit = true;
}

void SDLApplication::_ProcessEvents()
{
	_Exit = false;
	while (!_Exit)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event) && !_Exit)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Exit();
				break;
			case SDL_VIDEORESIZE:
				_VideoResizeEvent(event.resize);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				_KeyboardEvent(event.key);
				break;
			case SDL_MOUSEMOTION:
				_MouseMotionEvent(event.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				_MouseButtonEvent(event.button);
				break;
			}

			if (!_Exit)
			{
				_Update();
				//_Render();
			}
		}
	}
}

void SDLApplication::_VideoResizeEvent(const SDL_ResizeEvent& event)
{
	SetWindowSize(event.w, event.h);

	//Core::Instance()->GetRenderer()->Resize(0, 0, _Width, _Height);

	OnResize();
}

void SDLApplication::_KeyboardEvent(const SDL_KeyboardEvent& event)
{
	KeyCode code = SDLKeyCode(event.keysym.sym);
	KeyState state;
	
	if (event.state == SDL_PRESSED)
		state = KS_KeyPressed;
	else if (event.state == SDL_RELEASED)
		state = KS_KeyReleased;
	
	OnKeyboard(code, state);
}

void SDLApplication::_MouseMotionEvent(const SDL_MouseMotionEvent& event)
{
	OnMouseMove(event.x, event.y);
}

void SDLApplication::_MouseButtonEvent(const SDL_MouseButtonEvent& event)
{
	MouseButton mouseButton;
	MouseState mouseState;

	switch (event.button)
	{
	case SDL_BUTTON_LEFT: mouseButton = MB_Left; break;
	case SDL_BUTTON_MIDDLE: mouseButton = MB_Middle; break;
	case SDL_BUTTON_RIGHT: mouseButton = MB_Right; break;
	}

	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN: mouseState = MS_ButtonDown; break;
	case SDL_MOUSEBUTTONUP: mouseState = MS_ButtonUp; break;
	default: mouseState = MS_Axis; break;
	}

	OnMouseButton(mouseButton, mouseState, event.x, event.y);
}

void SDLApplication::_Update()
{
	// Time
	real64 _LastTime = _Time;
	_Time = SDL_GetTicks() * 0.001;
	_ElapsedTime = _Time - _LastTime;

	// Input
	_KeyState = SDL_GetKeyState(NULL);
	_MouseState = SDL_GetMouseState(NULL, NULL);

	// Center Mouse
	if (_CursorLocked)
	{
		SDL_WarpMouse(_Width / 2, _Height / 2);
	}

	OnUpdate(_ElapsedTime);
}

void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
	{
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
		else
		{
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32*)p = pixel;
        break;
    }
}

Uint32 ColourToSDL(byte* data, int32 x, int32 y, GPRBUFFER_DESC desc)
{
	Uint32 pixel = 0;

	int stride;
	int index;
	if (desc.elementType == GPRDECLTYPE_BYTE)
	{
		stride = sizeof(byte);
	}
	else if (desc.elementType == GPRDECLTYPE_INT)
	{
		stride = sizeof(int32);
	}
	else if (desc.elementType == GPRDECLTYPE_FLOAT)
	{
		stride = sizeof(real32);
		index = (y * stride*3) * desc.dimensions[0] + (x * stride*3);
		Colour32 colour = Colour32(
			*(real32*)&data[index],
			*(real32*)&data[index+stride],
			*(real32*)&data[index+stride*2]);
		pixel = colour.ToARGB();
	}

	return pixel;
}

void SDLApplication::Render()
{
	OnRender();

	// Lock the screen for direct access to the pixels
	if (SDL_LockSurface(_Screen) < 0)
		return;

	SoftwareRenderer* renderer = (SoftwareRenderer*) Core::Instance()->GetRenderer();
	OGPRBuffer* buffer = renderer->GetColourBuffer();

	GPRBUFFER_DESC desc;
	buffer->GetDesc(&desc);

#ifndef ARGB_COLOUR_BUFFER
	if (desc.nDimensions != 2 || desc.nComponents != 3)
		return;

	byte* data;
	if (buffer->Lock(GPRLOCK_READONLY, (void**)&data) == GPRRESULT_OK)
	{
		memcpy(_Screen->pixels, data, desc.dimensions[0] * desc.dimensions[1] * sizeof(Uint32));
		for (uint y = 0; y < desc.dimensions[1]; y++)
		{
			for (uint x = 0; x < desc.dimensions[0]; x++)
			{
				Uint32 pixel = ColourToSDL(data, x, y, desc);
				putpixel(_Screen, x, desc.dimensions[1]-y-1, pixel);
			}
		}

		buffer->Unlock((void**)&data);
	}

#else
	if (desc.nDimensions != 2 || desc.nComponents != 1)
		return;

	byte* data;
	if (buffer->Lock(GPRLOCK_READONLY, (void**)&data) == GPRRESULT_OK)
	{
		uint src = desc.dimensions[0]*desc.dimensions[1];
		uint dst = 0;
		for (uint y = 0; y < desc.dimensions[1]; y++)
		{
			src -= desc.dimensions[0];
			memcpy((Uint32*)_Screen->pixels+src,
			(Uint32*)data+dst,
			desc.dimensions[0] * sizeof(Uint32));
			dst += desc.dimensions[0];
		}
		//memcpy(_Screen->pixels, data, desc.dimensions[0] * desc.dimensions[1] * sizeof(Uint32));
		/*for (uint y = 0; y < desc.dimensions[1]; y++)
		{
			for (uint x = 0; x < desc.dimensions[0]; x++)
			{
				Uint32 pixel = ColourToSDL(data, x, y, desc);
				putpixel(_Screen, x, desc.dimensions[1]-y-1, pixel);
			}
		}*/

		buffer->Unlock((void**)&data);
	}
#endif

	SDL_UnlockSurface(_Screen);

	// Update just the part of the display that we've changed
	SDL_UpdateRect(_Screen, 0, 0, 0, 0);

	SDL_Flip(_Screen);
}
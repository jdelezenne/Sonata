/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_COMMON_H_
#define _SE_UI_COMMON_H_

#include "Core/Core.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Graphics/Font/Font.h"
#include "Graphics/Font/BitmapFontProvider.h"
#include "Graphics/Font/Text.h"

#define SE_ID_UISYSTEM SonataEngine::SE_ID(0x5eace528,0x85fb4fa4)

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_UI_EXPORT
#	else
#		ifdef SE_ENGINEUI
#			define SE_UI_EXPORT __declspec(dllexport)
#		else
#			define SE_UI_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_UI_EXPORT
#endif

namespace SonataEngine
{
	namespace UI
	{
		/** Orientation. */
		enum Orientation
		{
			/** Horizontal orientation. */
			Orientation_Horizontal,

			/** Vertical orientation. */
			Orientation_Vertical
		};

		/** Content alignment in a widget. */
		enum Alignment
		{
			/** Content is vertically aligned at the top,and horizontally aligned on the left. */
			Alignment_TopLeft,

			/** Content is vertically aligned at the top,and horizontally aligned at the center. */
			Alignment_TopCenter,

			/** Content is vertically aligned at the top,and horizontally aligned on the right. */
			Alignment_TopRight,

			/** Content is vertically aligned in the middle,and horizontally aligned on the left. */
			Alignment_MiddleLeft,

			/** Content is vertically aligned in the middle,and horizontally aligned at the center. */
			Alignment_MiddleCenter,

			/** Content is vertically aligned in the middle,and horizontally aligned on the right.*/ 
			Alignment_MiddleRight,

			/** Content is vertically aligned at the bottom,and horizontally aligned on the left. */
			Alignment_BottomLeft,

			/** Content is vertically aligned at the bottom,and horizontally aligned at the center. */
			Alignment_BottomCenter,

			/** Content is vertically aligned at the bottom,and horizontally aligned on the right. */
			Alignment_BottomRight
		};

		/** Image layout. */
		enum ImageLayout
		{
			/** The image is aligned across the widget's local rectangle. */
			ImageLayout_None,

			/** The image is tiled across the widget's local rectangle. */
			ImageLayout_Tile,

			/** The image is streched across the widget's local rectangle. */
			ImageLayout_Stretch,

			/** The image is enlarged within the widget's local rectangle. */
			ImageLayout_Scale
		};

		/** Frame location. */
		enum FrameLocation
		{
			FrameLocation_None,
			FrameLocation_Left,
			FrameLocation_Top,
			FrameLocation_Right,
			FrameLocation_Bottom,
			FrameLocation_TopLeft,
			FrameLocation_TopRight,
			FrameLocation_BottomLeft,
			FrameLocation_BottomRight
		};

		class SE_UI_EXPORT UpdateEventArgs : public EventArgs
		{
		public:
			UpdateEventArgs(real64 elapsedTime) :
				EventArgs(),
				_ElapsedTime(elapsedTime)
			{
			}

			/** Gets the elapsed time. */
			real64 ElapsedTime() const { return _ElapsedTime; }

		protected:
			real64 _ElapsedTime;
		};
		SE_DEFINE_EVENT(Update, UpdateEventArgs);

		typedef void DrawContext;

		/** Provides data for the drawing events. */
		class SE_UI_EXPORT DrawEventArgs : public EventArgs
		{
		public:
			DrawEventArgs(DrawContext* context, RectangleInt& clipRect) :
				EventArgs(),
				_Context(context),
				_ClipRectangle(clipRect)
			{
			}

			/** Gets the draw context. */
			DrawContext* Context() const { return _Context; }

			/** Gets the clipping rectangle. */
			RectangleInt ClipRectangle() const { return _ClipRectangle; }

		protected:
			DrawContext* _Context;
			RectangleInt _ClipRectangle;
		};
		SE_DEFINE_EVENT(Draw, DrawEventArgs);

		enum MouseButtons
		{
			MouseButtons_None = 0,
			MouseButtons_Left = (1<<0),
			MouseButtons_Right = (1<<1),
			MouseButtons_Middle = (1<<2)
		};

		enum ModifierKeys
		{
			ModifierKeys_None = 0,
			ModifierKeys_Control = (1<<0),
			ModifierKeys_Shift = (1<<1),
			ModifierKeys_Alt = (1<<2)
		};

		/** Provides data for the mouse events. */
		class SE_UI_EXPORT MouseEventArgs : public EventArgs
		{
		public:
			MouseEventArgs(MouseButtons button, int32 x,int32 y, int32 delta, ModifierKeys modifiers = ModifierKeys_None) :
				EventArgs(),
				_Button(button),
				_X(x),
				_Y(y),
				_Delta(delta),
				_Modifiers(modifiers)
			{
			}

			/** Gets which mouse button was pressed. */
			MouseButtons Button() const { return _Button; }

			/** Gets the amount the mouse wheel has rotated. */
			int32 Delta() const { return _Delta; }

			/** Gets the location of the mouse. */
			PointInt Location() const { return PointInt(_X, _Y); }

			/** Gets the x-coordinate of the mouse. */
			int32 X() const { return _X; }

			/** Gets the y-coordinate of the mouse. */
			int32 Y() const { return _Y; }

			/**
				Gets the modifier flags that indicate which combination of
				Ctrl, Shift and Alt was pressed.
			*/
			ModifierKeys Modifiers() const { return _Modifiers; }

		protected:
			MouseButtons _Button;
			int32 _Delta;
			int32 _X;
			int32 _Y;
			ModifierKeys _Modifiers;
		};
		SE_DEFINE_EVENT(Mouse, MouseEventArgs);

		/** Provides data for the key events. */
		class SE_UI_EXPORT KeyEventArgs : public EventArgs
		{
		public:
			KeyEventArgs(Input::KeyCode key, ModifierKeys modifiers = ModifierKeys_None) :
				EventArgs(),
				_Key(key),
				_Modifiers(modifiers)
			{
			}

			/** Gets the keyboard code of the key that was pressed. */
			Input::KeyCode Key() const { return _Key; }

			/**
				Gets the modifier flags that indicate which combination of
				Ctrl, Shift and Alt was pressed.
			*/
			ModifierKeys Modifiers() const { return _Modifiers; }

		protected:
			Input::KeyCode _Key;
			ModifierKeys _Modifiers;
		};
		SE_DEFINE_EVENT(Key, KeyEventArgs);

		/** Provides data for the character key events. */
		class KeyCharEventArgs : public EventArgs
		{
		public:
			KeyCharEventArgs(Char keyChar) :
				EventArgs(),
				_KeyChar(keyChar)
			{
			}

			/** Gets the character corresponding to the key pressed. */
			Char KeyChar() const { return _KeyChar; }

		protected:
			Char _KeyChar;
		};
		SE_DEFINE_EVENT(KeyChar, KeyCharEventArgs);

		SE_BEGIN_ENUM(Orientation);
			SE_Enum(Horizontal);
			SE_Enum(Vertical);
		SE_END_ENUM(Orientation);

		SE_BEGIN_ENUM(Alignment);
			SE_Enum(TopLeft);
			SE_Enum(TopCenter);
			SE_Enum(TopRight);
			SE_Enum(MiddleLeft);
			SE_Enum(MiddleCenter);
			SE_Enum(MiddleRight);
			SE_Enum(BottomLeft);
			SE_Enum(BottomCenter);
			SE_Enum(BottomRight);
		SE_END_ENUM(Alignment);

		SE_BEGIN_ENUM(ImageLayout);
			SE_Enum(None);
			SE_Enum(Tile);
			SE_Enum(Stretch);
			SE_Enum(Zoom);
		SE_END_ENUM(ImageLayout);

		SE_BEGIN_ENUM(FrameLocation);
			SE_Enum(None);
			SE_Enum(Left);
			SE_Enum(Top);
			SE_Enum(Right);
			SE_Enum(Bottom);
			SE_Enum(TopLeft);
			SE_Enum(TopRight);
			SE_Enum(BottomLeft);
			SE_Enum(BottomRight);
		SE_END_ENUM(FrameLocation);
	}
}

#endif

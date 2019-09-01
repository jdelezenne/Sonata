/*=============================================================================
Window.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Window.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Window);
		SE_IMPLEMENT_REFLECTION(Window);

		Window::Window() :
			super()
		{
		}

		Window::~Window()
		{
		}

		void Window::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void Window::DoRender()
		{
			super::DoRender();
		}

		void Window::OnMouseMove(MouseEventArgs& e)
		{
			super::OnMouseMove(e);
		}

		void Window::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void Window::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}
	}
}

/*=============================================================================
ButtonBase.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ButtonBase.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_ABSTRACT(ButtonBase);
		SE_IMPLEMENT_REFLECTION(ButtonBase);

		ButtonBase::ButtonBase() :
			super()
		{
			_Pushed = false;
			_TextAlignment = Alignment_TopLeft;

			_PushedColor = Color32::White;
			_HoverColor = Color32(211, 211, 211);

			_TextObj = new Text();
			_MouseHover = false;
		}

		ButtonBase::~ButtonBase()
		{
		}

		void ButtonBase::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void ButtonBase::DoRender()
		{
			super::DoRender();
		}

		void ButtonBase::OnMouseEnter(EventArgs& e)
		{
			super::OnMouseEnter(e);

			if (!GetEnabled() || !GetVisible())
				return;

			_MouseHover = true;

			if (UISystem::Instance()->GetCapture() == this)
			{
				_Pushed = true;
			}
		}

		void ButtonBase::OnMouseLeave(EventArgs& e)
		{
			super::OnMouseLeave(e);

			if (!GetEnabled() || !GetVisible())
				return;

			_MouseHover = false;

			if (UISystem::Instance()->GetCapture() == this)
			{
				_Pushed = false;
			}
		}

		void ButtonBase::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				_Pushed = true;
				UISystem::Instance()->SetCapture(this);

				//FIX
				UISystem::Instance()->SetFocusedWidget(NULL);
			}
		}

		void ButtonBase::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				if (GetPushed())
				{
					_Pushed = false;
					UISystem::Instance()->ReleaseCapture();

					if (_MouseHover)
						Click.Invoke(this, EventArgs::Empty);
				}
			}
		}

		void ButtonBase::OnKeyDown(KeyEventArgs& e)
		{
			super::OnKeyDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Key() == Input::Key_Space)
			{
				_Pushed = true;
			}
		}

		void ButtonBase::OnKeyUp(KeyEventArgs& e)
		{
			super::OnKeyUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Key() == Input::Key_Space)
			{
				_Pushed = false;

				Click.Invoke(this, EventArgs::Empty);
			}
		}
	}
}

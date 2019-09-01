/*=============================================================================
Dialog.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Dialog.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_ABSTRACT(Dialog);
		SE_IMPLEMENT_REFLECTION(Dialog);

		Dialog::Dialog() :
			super()
		{
			_HasFrame = true;
			_FrameSize = 2;
			_Movable = true;
			_Sizable = true;

			_BackgroundColor = Color32::Gray;

			_DialogState = DialogState_Normal;
			_Icon = NULL;
			_MenuBar = NULL;
			_Modal = false;
			_DialogResult = DialogResult_OK;
			_CancelButton = NULL;
			_AcceptButton = NULL;
			_ShowTitleBar = true;
			_TitleBarHeight = UISystem::DefaultSize + 2;
			_ShowMinimizeBox = false;
			_ShowMaximizeBox = false;
			_ShowCloseBox = true;
			_ShowIcon = false;
			_Icon = NULL;

			_TitleBarColor = Color32::Blue;
		}

		Dialog::~Dialog()
		{
		}

		void Dialog::Show(Widget* owner)
		{
			if (owner != NULL)
			{
				SetParent(owner);
			}
			else
			{
				Widget* root = UISystem::Instance()->GetCurrentWidget();
				if (root != NULL)
				{
					SetParent(root);
				}
			}
		}

		void Dialog::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void Dialog::DoRender()
		{
			super::DoRender();
		}

		void Dialog::OnMouseMove(MouseEventArgs& e)
		{
			super::OnMouseMove(e);
		}

		void Dialog::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void Dialog::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}
	}
}

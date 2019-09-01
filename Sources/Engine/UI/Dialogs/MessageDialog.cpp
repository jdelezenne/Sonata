/*=============================================================================
MessageDialog.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "MessageDialog.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(MessageDialog);
		SE_IMPLEMENT_REFLECTION(MessageDialog);

		MessageDialog::MessageDialog() :
			super()
		{
			_Modal = true;

			_Buttons = MessageDialogButtons_OK;
			_Icon = MessageDialogIcon_None;
			_DefaultButton = MessageDialogDefaultButton_Button1;
		}

		MessageDialog::~MessageDialog()
		{
		}
	}
}

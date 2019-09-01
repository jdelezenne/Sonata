/*=============================================================================
MessageDialog.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_MESSAGEDIALOG_H_
#define _SE_UI_MESSAGEDIALOG_H_

#include "UI/Common.h"
#include "UI/Dialog.h"
#include "UI/PushButton.h"

namespace SonataEngine
{
	namespace UI
	{
		/** The buttons to display on a MessageDialog. */
		enum MessageDialogButtons
		{
			/** The message dialog contains an OK button. */
			MessageDialogButtons_OK,
			
			/** The message dialog contains OK and Cancel buttons. */
			MessageDialogButtons_OKCancel,
			
			/** The message dialog contains Abort, Retry, and Ignore buttons. */
			MessageDialogButtons_AbortRetryIgnore,
			
			/** The message dialog contains Yes, No, and Cancel buttons. */
			MessageDialogButtons_YesNoCancel,
			
			/** The message dialog contains Yes and No buttons. */
			MessageDialogButtons_YesNo,
			
			/** The message dialog contains Retry and Cancel buttons. */
			MessageDialogButtons_RetryCancel
		};

		SE_BEGIN_ENUM(MessageDialogButtons);
			SE_Enum(OK);
			SE_Enum(OKCancel);
			SE_Enum(AbortRetryIgnore);
			SE_Enum(YesNoCancel);
			SE_Enum(YesNo);
			SE_Enum(RetryCancel);
		SE_END_ENUM(MessageDialogButtons);

		/** The icons to display on a MessageDialog. */
		enum MessageDialogIcon
		{
			MessageDialogIcon_None,
			MessageDialogIcon_Error,
			MessageDialogIcon_Question,
			MessageDialogIcon_Warning,
			MessageDialogIcon_Information
		};

		SE_BEGIN_ENUM(MessageDialogIcon);
			SE_Enum(None);
			SE_Enum(Error);
			SE_Enum(Question);
			SE_Enum(Warning);
			SE_Enum(Information);
		SE_END_ENUM(MessageDialogIcon);

		/** The default button on a MessageDialog. */
		enum MessageDialogDefaultButton
		{
			/** The first button on the message dialog is the default button. */
			MessageDialogDefaultButton_Button1,

			/** The second button on the message dialog is the default button. */
			MessageDialogDefaultButton_Button2,

			/** The third button on the message dialog is the default button. */
			MessageDialogDefaultButton_Button3
		};

		SE_BEGIN_ENUM(MessageDialogDefaultButton);
			SE_Enum(Button1);
			SE_Enum(Button2);
			SE_Enum(Button3);
		SE_END_ENUM(MessageDialogDefaultButton);

		/**
			@brief Displays a message dialog that can contain text, buttons, and icons.
		*/
		class SE_UI_EXPORT MessageDialog : public Dialog
		{
			SE_DECLARE_CLASS(MessageDialog, Dialog);

			SE_BEGIN_REFLECTION(MessageDialog);
			SE_END_REFLECTION(MessageDialog);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			MessageDialog();

			/** Destructor. */
			virtual ~MessageDialog();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the text to display in the message dialog. */
			const String& GetText() const { return _Text; }
			void SetText(const String& value) { _Text = value; }

			/** Gets or sets the text to display in the title bar of the message dialog. */
			const String& GetCaption() const { return _Caption; }
			void SetCaption(const String& value) { _Caption = value; }

			/** Gets or sets the buttons to display in the message dialog. */
			MessageDialogButtons GetButtons() const { return _Buttons; }
			void SetButtons(MessageDialogButtons value) { _Buttons = value; }

			/** Gets or sets the icon to display in the message dialog. */
			MessageDialogIcon GetIcon() const { return _Icon; }
			void SetIcon(MessageDialogIcon value) { _Icon = value; }

			/** Gets or sets the default button for the message dialog. */
			MessageDialogDefaultButton GetDefaultButton() const { return _DefaultButton; }
			void SetDefaultButton(MessageDialogDefaultButton value) { _DefaultButton = value; }
			//@}

			/** @name Operations. */
			//@{
			//@}

		protected:
			String _Text;
			String _Caption;
			MessageDialogButtons _Buttons;
			MessageDialogIcon _Icon;
			MessageDialogDefaultButton _DefaultButton;
		};
	}
}

#endif 

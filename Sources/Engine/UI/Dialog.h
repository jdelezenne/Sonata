/*=============================================================================
Dialog.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIALOG_H_
#define _SE_UI_DIALOG_H_

#include "UI/Common.h"
#include "UI/ContainerPanel.h"
#include "UI/PushButton.h"

namespace SonataEngine
{
	namespace UI
	{
		class MenuBar;

		/** Identifiers to indicate the return value of a dialog box. */
		enum DialogResult
		{
			DialogResult_OK,
			DialogResult_Cancel,
		};

		/** Specifies how a dialog is displayed. */
		enum DialogState
		{
			DialogState_Normal,
			DialogState_Minimized,
			DialogState_Maximized
		};

		/**
			@brief Represents a dialog box.
		*/
		class SE_UI_EXPORT Dialog : public ContainerPanel
		{
			SE_DECLARE_ABSTRACT(Dialog, ContainerPanel);

			SE_BEGIN_REFLECTION(Dialog);
			SE_END_REFLECTION(Dialog);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Dialog();

			/** Destructor. */
			virtual ~Dialog();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the dialog state. */
			DialogState GetDialogState() const { return _DialogState; }

			/** Gets or sets the primary menu container for the dialog. */
			MenuBar* GetMenuBar() const { return _MenuBar; }
			void SetMenuBar(MenuBar* value) { _MenuBar = value; }

			/** Gets a value indicating whether this dialog is displayed modally. */
			bool GetModal() const { return _Modal; }
			void SetModal(bool value) { _Modal = value; }
			void SetDialogState(DialogState value) { _DialogState = value; }

			/** Gets or sets the dialog result for the dialog. */
			DialogResult GetDialogResult() const { return _DialogResult; }
			void SetDialogResult(DialogResult value) { _DialogResult = value; }

			/** Gets or sets the button on the dialog that is clicked when the user presses the Enter key. */
			PushButton* GetAcceptButton() const { return _AcceptButton; }
			void SetAcceptButton(PushButton* value) { _AcceptButton = value; }

			/** Gets or sets the button control that is clicked when the user presses the Escape key. */
			PushButton* GetCancelButton() const { return _CancelButton; }
			void SetCancelButton(PushButton* value) { _CancelButton = value; }

			/** Gets or sets a value indicating whether a title bar is displayed. */
			bool GetShowTitleBar() const { return _ShowTitleBar; }
			void SetShowTitleBar(bool value) { _ShowTitleBar = value; }

			/** Gets or sets the title of the dialog. */
			String GetTitle() const { return _Title; }
			void SetTitle(String value) { _Title = value; }

			/** Gets or sets the height of the title bar of the dialog. */
			int32 GetTitleBarHeight() const { return _TitleBarHeight; }
			void SetTitleBarHeight(int32 value) { _TitleBarHeight = value; }

			/** Gets or sets a value indicating whether the Minimize button is displayed in the title bar of the dialog. */
			bool GetShowMinimizeBox() const { return _ShowMinimizeBox; }
			void SetShowMinimizeBox(bool value) { _ShowMinimizeBox = value; }

			/** Gets or sets a value indicating whether the Maximize button is displayed in the title bar of the dialog. */
			bool GetShowMaximizeBox() const { return _ShowMaximizeBox; }
			void SetShowMaximizeBox(bool value) { _ShowMaximizeBox = value; }

			/** Gets or sets a value indicating whether the Close button is displayed in the title bar of the dialog. */
			bool GetShowCloseBox() const { return _ShowCloseBox; }
			void SetShowCloseBox(bool value) { _ShowCloseBox = value; }

			/** Gets or sets a value indicating whether an icon is displayed in the title bar of the dialog. */
			bool GetShowIcon() const { return _ShowIcon; }
			void SetShowIcon(bool value) { _ShowIcon = value; }

			/** Gets or sets the icon for the dialog. */
			Texture* GetIcon() const { return _Icon; }
			void SetIcon(Texture* value) { _Icon = value; }

			/** Gets or sets the color of the title bar of the dialog. */
			Color32 GetTitleBarColor() const { return _TitleBarColor; }
			void SetTitleBarColor(Color32 value) { _TitleBarColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Displays a message dialog with the specified text, caption, buttons, icon, and default button. */
			virtual void Show(Widget* owner);

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the dialog is activated. */
			DefaultEvent Activated;

			/** Occurs when the dialog loses focus and is no longer the active dialog. */
			DefaultEvent Deactivated;

			/** Occurs before the dialog is closed. */
			DefaultEvent Closing;

			/** Occurs after the dialog is closed. */
			DefaultEvent Closed;

			/** Occurs after the modal dialog is closed. */
			DefaultEvent ModalDialogClosed;
			//@}

		protected:
			DialogState _DialogState;
			MenuBar* _MenuBar;
			bool _Modal;
			DialogResult _DialogResult;
			PushButton* _CancelButton;
			PushButton* _AcceptButton;
			bool _ShowTitleBar;
			String _Title;
			int32 _TitleBarHeight;
			bool _ShowMinimizeBox;
			bool _ShowMaximizeBox;
			bool _ShowCloseBox;
			bool _ShowIcon;
			Texture* _Icon;

			Color32 _TitleBarColor;
		};
	}
}

#endif 

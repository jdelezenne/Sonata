/*=============================================================================
Window.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_WINDOW_H_
#define _SE_UI_WINDOW_H_

#include "UI/Common.h"
#include "UI/Dialog.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Represents a window that can function as a container for child dialogs.
		*/
		class SE_UI_EXPORT Window : public Dialog
		{
			SE_DECLARE_CLASS(Window, Dialog);

			SE_BEGIN_REFLECTION(Window);
			SE_END_REFLECTION(Window);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Window();

			/** Destructor. */
			virtual ~Window();
			//@}

			/** @name Properties. */
			//@{
			/** Gets an array of dialogs that represent the child dialogs that are parented to this dialog. */
			Array<Dialog*>& GetDialogChildren() { return _DialogChildren; }

			//@}

			/** @name Operations. */
			//@{
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
			//@}

		protected:
			Array<Dialog*> _DialogChildren;
		};
	}
}

#endif 

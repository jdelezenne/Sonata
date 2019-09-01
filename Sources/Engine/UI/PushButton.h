/*=============================================================================
PushButton.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_PUSHBUTTON_H_
#define _SE_UI_PUSHBUTTON_H_

#include "UI/Common.h"
#include "UI/ButtonBase.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Base class for button widgets.
		*/
		class SE_UI_EXPORT PushButton : public ButtonBase
		{
			SE_DECLARE_CLASS(PushButton, ButtonBase);

			SE_BEGIN_REFLECTION(PushButton);
			SE_END_REFLECTION(PushButton);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			PushButton();

			/** Destructor. */
			virtual ~PushButton();
			//@}

			/** @name Properties. */
			//@{
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
		};
	}
}

#endif 

/*=============================================================================
RadioButton.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_RADIOBUTTON_H_
#define _SE_UI_RADIOBUTTON_H_

#include "UI/Common.h"
#include "UI/ButtonBase.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Represents a radio button.

			@todo ButtonGroup
		*/
		class SE_UI_EXPORT RadioButton : public ButtonBase
		{
			SE_DECLARE_CLASS(RadioButton, ButtonBase);

			SE_BEGIN_REFLECTION(RadioButton);
				SE_Field(_Checked, bool, Public);
				SE_Field(_ButtonGroup, int32, Public);
				SE_Field(_CheckAlignment, int32, Public);
				SE_Field(_CheckedColor, Color32, Public);
			SE_END_REFLECTION(RadioButton);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			RadioButton();

			/** Destructor. */
			virtual ~RadioButton();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or set a value indicating whether the Radio is in the checked state. */
			bool GetChecked() const { return _Checked; }
			void SetChecked(bool value);

			/** Gets or sets the alignment of the check mark on the Radio button. */
			Alignment GetCheckAlignment() const { return _CheckAlignment; }
			void SetCheckAlignment(Alignment value) { _CheckAlignment = value; }

			/** Gets or sets the button group of the Radio button. */
			int32 GetButtonGroup() const { return _ButtonGroup; }
			void SetButtonGroup(int32 value) { _ButtonGroup = value; }

			Color32 GetCheckedColor() const { return _CheckedColor; }
			void SetCheckedColor(const Color32& value) { _CheckedColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnMouseEnter(EventArgs& e);
			virtual void OnMouseLeave(EventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			virtual void OnKeyUp(KeyEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the value of the Checked property changes. */
			DefaultEvent CheckedChanged;
			//@}

		protected:
			bool _Checked;
			int32 _ButtonGroup;
			Alignment _CheckAlignment;

			Color32 _CheckedColor;
		};
	}
}

#endif 

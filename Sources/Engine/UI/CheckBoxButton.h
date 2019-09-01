/*=============================================================================
CheckBoxButton.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_CHECKBOXBUTTON_H_
#define _SE_UI_CHECKBOXBUTTON_H_

#include "UI/Common.h"
#include "UI/ButtonBase.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Represents a CheckBox.
		*/
		class SE_UI_EXPORT CheckBoxButton : public ButtonBase
		{
			SE_DECLARE_CLASS(CheckBoxButton, ButtonBase);

			SE_BEGIN_REFLECTION(CheckBoxButton);
				SE_Field(_Checked, bool, Public);
				SE_Field(_CheckAlignment, int32, Public);
				SE_Field(_CheckedColor, Color32, Public);
			SE_END_REFLECTION(CheckBoxButton);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			CheckBoxButton();

			/** Destructor. */
			virtual ~CheckBoxButton();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the alignment of the check mark on the CheckBox button. */
			Alignment GetCheckAlignment() const { return _CheckAlignment; }
			void SetCheckAlignment(Alignment value) { _CheckAlignment = value; }

			/** Gets or set a value indicating whether the CheckBox is in the checked state. */
			bool GetChecked() const { return _Checked; }
			void SetChecked(bool value);

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
			Alignment _CheckAlignment;

			Color32 _CheckedColor;
		};
	}
}

#endif 

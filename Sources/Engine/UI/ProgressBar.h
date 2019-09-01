/*=============================================================================
ProgressBar.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_PROGRESSBAR_H_
#define _SE_UI_PROGRESSBAR_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Represents a progress bar widget.

			@todo Show Min/Max/Value.
		*/
		class SE_UI_EXPORT ProgressBar : public Widget
		{
			SE_DECLARE_CLASS(ProgressBar, Widget);

			SE_BEGIN_REFLECTION(ProgressBar);
				SE_Field(_Orientation, Orientation, Public);
				SE_Field(_Minimum, int32, Public);
				SE_Field(_Maximum, int32, Public);
				SE_Field(_Step, int32, Public);
				SE_Field(_Value, int32, Public);
				SE_Field(_BarColor, Color32, Public);
			SE_END_REFLECTION(ProgressBar);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ProgressBar();

			/** Destructor. */
			virtual ~ProgressBar();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the horizontal or vertical orientation of the progress bar. */
			Orientation GetOrientation() const { return _Orientation; }
			void SetOrientation(Orientation value) { _Orientation = value; }

			/** Gets or sets the minimum value of the range of the progress bar. */
			int32 GetMinimum() const { return _Minimum; }
			void SetMinimum(int32 value);

			/** Gets or sets the maximum value of the range of the progress bar. */
			int32 GetMaximum() const { return _Maximum; }
			void SetMaximum(int32 value);

			/** Gets or sets the amount by which to increase the current position of the progress bar.  */
			int32 GetStep() const { return _Step; }
			void SetStep(int32 value) { _Step = value; }

			/** Gets or sets the current position of the progress bar. */
			int32 GetValue() const { return _Value; }
			void SetValue(int32 value);

			Color32 GetBarColor() const { return _BarColor; }
			void SetBarColor(Color32 value) { _BarColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Sets the minimum and maximum values of the progress bar. */
			void SetRange(int32 minimum, int32 maximum);

			/** Advances the current position of the progress bar by the specified amount. */
			void Increment(int32 value);

			/** Advances the current position of the progress bar. */
			void Step();

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the value changes. */
			DefaultEvent ValueChanged;
			//@}

		protected:
			Orientation _Orientation;
			int32 _Minimum;
			int32 _Maximum;
			int32 _Step;
			int32 _Value;

			Color32 _BarColor;
		};
	}
}

#endif 

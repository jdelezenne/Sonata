/*=============================================================================
Slider.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_SLIDER_H_
#define _SE_UI_SLIDER_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		enum TickStyle
		{
			/** No tick marks appear in the control. */
			TickStyle_None,

			/** Tick marks are located on the top of a horizontal slider or on the left of a vertical slider. */
			TickStyle_TopLeft,

			/** Right Tick marks are located on the bottom of a horizontal slider or on the right side of a vertical slider. */
			TickStyle_BottomRight,

			/** Tick marks are located on both sides of the control. */
			TickStyle_Both
		};

		/**
			@brief Represents a slider widget.

			@todo
				Replace by SliderBase, SliderStandard, SliderFlat objects.
				Show Min/Max/Value.
		*/
		class SE_UI_EXPORT Slider : public Widget
		{
			SE_DECLARE_CLASS(Slider, Widget);

			SE_BEGIN_REFLECTION(Slider);
				SE_Field(_Orientation, Orientation, Public);
				SE_Field(_Minimum, int32, Public);
				SE_Field(_Maximum, int32, Public);
				SE_Field(_Value, int32, Public);
				SE_Field(_TickFrequency, int32, Public);
				SE_Field(_TickStyle, int32, Public);
				SE_Field(_BarColor, Color32, Public);
			SE_END_REFLECTION(Slider);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Slider();

			/** Destructor. */
			virtual ~Slider();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the horizontal or vertical orientation of the slider. */
			Orientation GetOrientation() const { return _Orientation; }
			void SetOrientation(Orientation value) { _Orientation = value; }

			/** Gets or sets the minimum value of the range of the slider. */
			int32 GetMinimum() const { return _Minimum; }
			void SetMinimum(int32 value);

			/** Gets or sets the maximum value of the range of the slider. */
			int32 GetMaximum() const { return _Maximum; }
			void SetMaximum(int32 value);

			/** Gets or sets the current position of the slider. */
			int32 GetValue() const { return _Value; }
			void SetValue(int32 value);

			/** Gets or sets a value that specifies the delta between ticks drawn on the control. */
			int32 GetTickFrequency() const { return _TickFrequency; }
			void SetTickFrequency(int32 value) { _TickFrequency = value; }

			/** Gets or sets a value indicating how to display the tick marks on the slider. */
			TickStyle GetTickStyle() const { return _TickStyle; }
			void SetTickStyle(TickStyle value) { _TickStyle = value; }

			Color32 GetBarColor() const { return _BarColor; }
			void SetBarColor(Color32 value) { _BarColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Sets the minimum and maximum values of the slider. */
			void SetRange(int32 minimum, int32 maximum);

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the value changes. */
			DefaultEvent ValueChanged;
			//@}

		protected:
			void UpdateTickFromPoint(const PointInt& value);

		protected:
			Orientation _Orientation;
			int32 _Minimum;
			int32 _Maximum;
			int32 _Value;
			int32 _TickFrequency;
			TickStyle _TickStyle;

			Color32 _BarColor;
		};
	}
}

#endif 

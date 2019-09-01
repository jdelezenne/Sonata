/*=============================================================================
ScrollBar.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_SCROLLBAR_H_
#define _SE_UI_SCROLLBAR_H_

#include "UI/Common.h"
#include "UI/Widget.h"
#include "UI/PushButton.h"

namespace SonataEngine
{
	namespace UI
	{
		/** Specifies the type of action used to raise the Scroll event. */
		enum ScrollEventType
		{
			/** The scroll box was moved a small distance. */
			ScrollEventType_SingleDecrement,

			/** The scroll box was moved a small distance. */
			ScrollEventType_SingleIncrement,

			/** The scroll box moved a large distance. */
			ScrollEventType_PageDecrement,

			/** The scroll box moved a large distance. */
			ScrollEventType_PageIncrement,

			/** The scroll box was moved. */
			ScrollEventType_ThumbPosition,
			
			/** The scroll box is currently being moved. */
			ScrollEventType_ThumbTrack,
			
			/** The scroll box was moved to the Minimum position. */
			ScrollEventType_First,

			/** The scroll box was moved to the Maximum position.  */
			ScrollEventType_Last,

			/** The scroll box has stopped moving. */
			ScrollEventType_EndScroll
		};

		/** Provides data for the scroll events. */
		class SE_UI_EXPORT ScrollEventArgs : public EventArgs
		{
		public:
			ScrollEventArgs(ScrollEventType type, int32 oldValue, int32 newValue, Orientation orientation) :
				EventArgs(),
				_Type(type),
				_OldValue(oldValue),
				_NewValue(newValue),
				_Orientation(orientation)
			{
			}

			/** Gets the type of scroll event that occurred. */
			ScrollEventType Type() const { return _Type; }

			/** Gets the old Value of the scroll bar. */
			int32 OldValue() const { return _OldValue; }

			/** Gets or sets the new Value of the scroll bar. */
			int32 NewValue() const { return _NewValue; }
			void NewValue(int32 value) { _NewValue = value; }

			/** Gets the scroll bar orientation that raised the Scroll event. */
			UI::Orientation Orientation() const { return _Orientation; }

		protected:
			ScrollEventType _Type;
			int32 _OldValue;
			int32 _NewValue;
			UI::Orientation _Orientation;
		};

		/**
			@brief Represents a scroll bar widget.

			@todo
				Timers for value changes.
				Fix ValueFromPoint.
		*/
		class SE_UI_EXPORT ScrollBar : public Widget
		{
			SE_DECLARE_CLASS(ScrollBar, Widget);

			SE_BEGIN_REFLECTION(ScrollBar);
				SE_Field(_Orientation, Orientation, Public);
				SE_Field(_Minimum, int32, Public);
				SE_Field(_Maximum, int32, Public);
				SE_Field(_SingleStep, int32, Public);
				SE_Field(_PageStep, int32, Public);
				SE_Field(_Value, int32, Public);
				SE_Field(_BarColor, Color32, Public);
			SE_END_REFLECTION(ScrollBar);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ScrollBar();

			/** Destructor. */
			virtual ~ScrollBar();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the horizontal or vertical orientation of the ScrollBar. */
			Orientation GetOrientation() const { return _Orientation; }
			void SetOrientation(Orientation value) { _Orientation = value; }

			/** Gets or sets the minimum value of the range of the ScrollBar. */
			int32 GetMinimum() const { return _Minimum; }
			void SetMinimum(int32 value);

			/** Gets or sets the maximum value of the range of the ScrollBar. */
			int32 GetMaximum() const { return _Maximum; }
			void SetMaximum(int32 value);

			/** Gets or sets the single step value. */
			int32 GetSingleStep() const { return _SingleStep; }
			void SetSingleStep(int32 value) { _SingleStep = value; }

			/** Gets or sets the page step value. */
			int32 GetPageStep() const { return _PageStep; }
			void SetPageStep(int32 value) { _PageStep = value; }

			/** Gets or sets the current position of the ScrollBar. */
			int32 GetValue() const { return _Value; }
			void SetValue(int32 value);

			Color32 GetBarColor() const { return _BarColor; }
			void SetBarColor(Color32 value) { _BarColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Sets the minimum and maximum values of the ScrollBar. */
			void SetRange(int32 minimum, int32 maximum);

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnSize(EventArgs& e);
			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseWheel(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the value changes. */
			DefaultEvent ValueChanged;

			/** Occurs when the scroll thumb has been moved. */
			DefaultEvent Scroll;
			//@}

		protected:
			void UpdateLayout();
			void UpdateValueFromPoint(const PointInt& value);
			void SetThumbValue(int32 value);

			virtual bool OnDecrementButtonClick(Object* sender, const EventArgs& e);
			virtual bool OnIncrementButtonClick(Object* sender, const EventArgs& e);
			virtual bool OnThumbButtonMouseMove(Object* sender, const MouseEventArgs& e);
			virtual bool OnThumbButtonMouseDown(Object* sender, const MouseEventArgs& e);
			virtual bool OnThumbButtonMouseUp(Object* sender, const MouseEventArgs& e);

			enum ScrollButtonType
			{
				ScrollButtonType_LeftHorizontal,
				ScrollButtonType_RightHorizontal,
				ScrollButtonType_TopVertical,
				ScrollButtonType_BottomVertical
			};

			class ScrollBarButton : public PushButton
			{
			public:
				ScrollBarButton();
				virtual ~ScrollBarButton();

				ScrollButtonType GetButtonType() const { return _ButtonType; }
				void SetButtonType(ScrollButtonType value) { _ButtonType = value; }

				virtual void DoRender();

			protected:
				ScrollButtonType _ButtonType;
			};

		protected:
			Orientation _Orientation;
			int32 _Minimum;
			int32 _Maximum;
			int32 _SingleStep;
			int32 _PageStep;
			int32 _Value;

			ScrollBarButton* _DecrementButton;
			ScrollBarButton* _IncrementButton;
			PushButton* _ThumbButton;
			bool _Moving;
			PointInt _MovingOrigin;

			Color32 _BarColor;
		};
	}
}

#endif 

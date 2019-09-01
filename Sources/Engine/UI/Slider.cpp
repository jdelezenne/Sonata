/*=============================================================================
Slider.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Slider.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Slider);
		SE_IMPLEMENT_REFLECTION(Slider);

		Slider::Slider() :
			Widget()
		{
			_Orientation = Orientation_Horizontal;
			_Minimum = 0;
			_Maximum = 100;
			_Value = 0;
			_TickFrequency = 1;
			_TickStyle = TickStyle_None;

			_BarColor = Color32::Blue;
		}

		Slider::~Slider()
		{
		}

		void Slider::SetMinimum(int32 value)
		{
			_Minimum = value;
			SetValue(GetValue());
		}

		void Slider::SetMaximum(int32 value)
		{
			_Maximum = value;
			SetValue(GetValue());
		}

		void Slider::SetValue(int32 value)
		{
			int32 newValue = Math::Clamp(value, GetMinimum(), GetMaximum());
			bool changed = (newValue != GetValue());
			_Value = newValue;

			if (changed)
			{
				ValueChanged.Invoke(this, EventArgs::Empty);
			}
		}

		void Slider::SetRange(int32 minimum, int32 maximum)
		{
			SetMinimum(minimum);
			SetMaximum(maximum);
		}

		void Slider::UpdateTickFromPoint(const PointInt& value)
		{
			if (GetOrientation() == Orientation_Horizontal)
				SetValue(((real32)value.X / (real32)GetWidth()) * (GetMaximum() - GetMinimum()) + GetMinimum());
			else// if (GetOrientation() == Orientation_Vertical)
				SetValue(((real32)value.Y / (real32)GetHeight()) * (GetMaximum() - GetMinimum()) + GetMinimum());
		}

		void Slider::DoUpdate(real64 elapsed)
		{
			Widget::DoUpdate(elapsed);
		}

		void Slider::DoRender()
		{
			Widget::DoRender();

			if (GetValue() > GetMinimum() && GetMaximum() > GetMinimum())
			{
				RectangleInt barRect = LocalToGlobal(GetClientRectangle());

				real32 ratio = (real32)(GetValue() - GetMinimum()) /
					(real32)(GetMaximum() - GetMinimum());

				if (GetOrientation() == Orientation_Horizontal)
					barRect.Width *= ratio;
				else// if (GetOrientation() == Orientation_Vertical)
					barRect.Height *= ratio;

				UISystem::Instance()->FillRectangle(GetBarColor(), barRect);
			}
		}

		void Slider::OnMouseMove(MouseEventArgs& e)
		{
			Widget::OnMouseMove(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (UISystem::Instance()->GetCapture() == this)
			{
				UpdateTickFromPoint(GlobalToLocal(e.Location()));
			}
		}

		void Slider::OnMouseDown(MouseEventArgs& e)
		{
			Widget::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				UISystem::Instance()->SetCapture(this);

				UpdateTickFromPoint(GlobalToLocal(e.Location()));
			}
		}

		void Slider::OnMouseUp(MouseEventArgs& e)
		{
			Widget::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				UISystem::Instance()->ReleaseCapture();
			}
		}

		void Slider::OnKeyDown(KeyEventArgs& e)
		{
			Widget::OnKeyDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Key() == Input::Key_Left || e.Key() == Input::Key_Up)
			{
				SetValue(GetValue() - 1);
			}
			else if (e.Key() == Input::Key_Right || e.Key() == Input::Key_Down)
			{
				SetValue(GetValue() + 1);
			}
		}
	}
}

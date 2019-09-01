/*=============================================================================
ProgressBar.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ProgressBar.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(ProgressBar);
		SE_IMPLEMENT_REFLECTION(ProgressBar);

		ProgressBar::ProgressBar() :
			Widget()
		{
			_Orientation = Orientation_Horizontal;
			_Minimum = 0;
			_Maximum = 100;
			_Step = 1;
			_Value = 0;

			_BarColor = Color32::Blue;
		}

		ProgressBar::~ProgressBar()
		{
		}

		void ProgressBar::SetMinimum(int32 value)
		{
			_Minimum = value;
			SetValue(GetValue());
		}

		void ProgressBar::SetMaximum(int32 value)
		{
			_Maximum = value;
			SetValue(GetValue());
		}

		void ProgressBar::SetValue(int32 value)
		{
			int32 newValue = Math::Clamp(value, GetMinimum(), GetMaximum());
			bool changed = (newValue != GetValue());
			_Value = newValue;

			if (changed)
			{
				ValueChanged.Invoke(this, EventArgs::Empty);
			}
		}

		void ProgressBar::SetRange(int32 minimum, int32 maximum)
		{
			SetMinimum(minimum);
			SetMaximum(maximum);
		}

		void ProgressBar::Increment(int32 value)
		{
			SetValue(GetValue() + value);
		}

		void ProgressBar::Step()
		{
			SetValue(GetValue() + GetStep());
		}

		void ProgressBar::DoUpdate(real64 elapsed)
		{
			Widget::DoUpdate(elapsed);
		}

		void ProgressBar::DoRender()
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
	}
}

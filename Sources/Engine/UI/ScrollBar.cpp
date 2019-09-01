/*=============================================================================
ScrollBar.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ScrollBar.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(ScrollBar);
		SE_IMPLEMENT_REFLECTION(ScrollBar);

		ScrollBar::ScrollBar() :
			super()
		{
			_Rectangle.Width = UISystem::DefaultSize;
			_Rectangle.Height = UISystem::DefaultSize;
			_BackgroundColor = Color32(212.0f/255.0f, 208.0f/255.0f, 200.0f/255.0f);

			_Orientation = Orientation_Horizontal;
			_Minimum = 0;
			_Maximum = 100;
			_SingleStep = 1;
			_PageStep = 10;
			_Value = 0;

			_DecrementButton = new ScrollBarButton();
			_IncrementButton = new ScrollBarButton();
			_ThumbButton = new PushButton();
			_Moving = false;

			Children().Add(_DecrementButton);
			Children().Add(_IncrementButton);
			Children().Add(_ThumbButton);

			_DecrementButton->Click += new DefaultEventDelegateM<ScrollBar>(this, &ScrollBar::OnDecrementButtonClick);
			_IncrementButton->Click += new DefaultEventDelegateM<ScrollBar>(this, &ScrollBar::OnIncrementButtonClick);
			_ThumbButton->MouseMove += new MouseEventDelegateM<ScrollBar>(this, &ScrollBar::OnThumbButtonMouseMove);
			_ThumbButton->MouseDown += new MouseEventDelegateM<ScrollBar>(this, &ScrollBar::OnThumbButtonMouseDown);
			_ThumbButton->MouseUp += new MouseEventDelegateM<ScrollBar>(this, &ScrollBar::OnThumbButtonMouseUp);

			_BarColor = Color32::Blue;

			UpdateLayout();
		}

		ScrollBar::~ScrollBar()
		{
			//SE_DELETE(_DecrementButton);
			//SE_DELETE(_IncrementButton);
			//SE_DELETE(_ThumbButton);
		}

		void ScrollBar::SetMinimum(int32 value)
		{
			_Minimum = value;
			SetValue(GetValue());
		}

		void ScrollBar::SetMaximum(int32 value)
		{
			_Maximum = value;
			SetValue(GetValue());
		}

		void ScrollBar::SetValue(int32 value)
		{
			int32 newValue = Math::Clamp(value, GetMinimum(), GetMaximum());
			bool changed = (newValue != GetValue());
			_Value = newValue;

			UpdateLayout();

			if (changed)
			{
				ValueChanged.Invoke(this, EventArgs::Empty);
			}
		}

		void ScrollBar::SetRange(int32 minimum, int32 maximum)
		{
			SetMinimum(minimum);
			SetMaximum(maximum);
		}

		void ScrollBar::UpdateLayout()
		{
			SizeInt buttonSize;
			SizeInt thumbSize;

			real32 page = (real32)GetPageStep() /
				(real32)(GetMaximum() - GetMinimum());

			real32 ratio = (real32)(GetValue() - GetMinimum()) /
				(real32)(GetMaximum() - GetMinimum());

			if (GetOrientation() == Orientation_Horizontal)
			{
				buttonSize = SizeInt(GetHeight(), GetHeight());
				int32 scrollingRange = (GetWidth() -  2 * GetHeight());
				thumbSize = SizeInt(scrollingRange * page, GetHeight());

				_DecrementButton->SetPosition(PointInt(0, 0));
				_IncrementButton->SetPosition(PointInt(GetWidth() - GetHeight(), 0));
				_ThumbButton->SetPosition(PointInt(
					GetHeight() + Math::Min((int32)(scrollingRange * ratio), scrollingRange - thumbSize.Width), 0));
			}
			else// if (GetOrientation() == Orientation_Vertical)
			{
				buttonSize = SizeInt(GetWidth(), GetWidth());
				int32 scrollingRange = (GetHeight() -  2 * GetWidth());
				thumbSize = SizeInt(GetWidth(), scrollingRange * page);

				_DecrementButton->SetPosition(PointInt(0, 0));
				_IncrementButton->SetPosition(PointInt(0, GetHeight() - GetWidth()));
				_ThumbButton->SetPosition(PointInt(0,
					GetWidth() + Math::Min((int32)(scrollingRange * ratio), scrollingRange - thumbSize.Height)));
			}

			_DecrementButton->SetSize(buttonSize);
			_IncrementButton->SetSize(buttonSize);
			_ThumbButton->SetSize(thumbSize);
		}

		void ScrollBar::UpdateValueFromPoint(const PointInt& value)
		{
			int32 oldValue = GetValue();
			PointInt diff = PointInt(value.X - _MovingOrigin.X, value.Y - _MovingOrigin.Y);
			_MovingOrigin = value;

			int32 range = (GetMaximum() - GetMinimum());

			if (GetOrientation() == Orientation_Horizontal)
			{
				int32 scrollingRange = (GetWidth() -  2 * GetHeight());
				SetThumbValue(oldValue + ((real32)diff.X / (real32)scrollingRange) * range);
			}
			else// if (GetOrientation() == Orientation_Vertical)
			{
				int32 scrollingRange = (GetHeight() -  2 * GetWidth());
				SetThumbValue(oldValue + ((real32)diff.Y / (real32)scrollingRange) * range);
			}

			UpdateLayout();

			if (oldValue != GetValue())
			{
				Scroll.Invoke(this, ScrollEventArgs(ScrollEventType_ThumbPosition,
					oldValue, GetValue(), GetOrientation()));
			}
		}

		void ScrollBar::SetThumbValue(int32 value)
		{
			SetValue(Math::Min(value, GetMaximum() - GetPageStep() + 1));
		}
		
		bool ScrollBar::OnDecrementButtonClick(Object* sender, const EventArgs& e)
		{
			int32 oldValue = GetValue();
			SetThumbValue(oldValue - GetSingleStep());

			UpdateLayout();

			if (oldValue != GetValue())
			{
				Scroll.Invoke(this, ScrollEventArgs(ScrollEventType_SingleDecrement,
					oldValue, GetValue(), GetOrientation()));
			}

			return true;
		}
		
		bool ScrollBar::OnIncrementButtonClick(Object* sender, const EventArgs& e)
		{
			int32 oldValue = GetValue();
			SetThumbValue(oldValue + GetSingleStep());

			UpdateLayout();

			if (oldValue != GetValue())
			{
				Scroll.Invoke(this, ScrollEventArgs(ScrollEventType_SingleIncrement,
					oldValue, GetValue(), GetOrientation()));
			}

			return true;
		}

		bool ScrollBar::OnThumbButtonMouseMove(Object* sender, const MouseEventArgs& ee)
		{
			const MouseEventArgs e = (const MouseEventArgs&)ee;

			if (UISystem::Instance()->GetCapture() == _ThumbButton)
			{
				if (_Moving)
				{
					UpdateValueFromPoint(GlobalToLocal(e.Location()));
				}
			}

			return true;
		}

		bool ScrollBar::OnThumbButtonMouseDown(Object* sender, const MouseEventArgs& ee)
		{
			const MouseEventArgs e = (const MouseEventArgs&)ee;

			if (e.Button() == MouseButtons_Left)
			{
				_Moving = true;
				_MovingOrigin = GlobalToLocal(e.Location());
			}

			return true;
		}

		bool ScrollBar::OnThumbButtonMouseUp(Object* sender, const MouseEventArgs& ee)
		{
			const MouseEventArgs e = (const MouseEventArgs&)ee;

			if (e.Button() == MouseButtons_Left)
			{
				_Moving = false;
			}

			return true;
		}

		void ScrollBar::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void ScrollBar::DoRender()
		{
			super::DoRender();

			if (GetOrientation() == Orientation_Horizontal)
			{
				_DecrementButton->SetButtonType(ScrollButtonType_LeftHorizontal);
				_IncrementButton->SetButtonType(ScrollButtonType_RightHorizontal);
			}
			else// if (GetOrientation() == Orientation_Vertical)
			{
				_DecrementButton->SetButtonType(ScrollButtonType_TopVertical);
				_IncrementButton->SetButtonType(ScrollButtonType_BottomVertical);
			}
		}

		void ScrollBar::OnSize(EventArgs& e)
		{
			super::OnSize(e);

			UpdateLayout();
		}

		void ScrollBar::OnMouseMove(MouseEventArgs& e)
		{
			super::OnMouseMove(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void ScrollBar::OnMouseWheel(MouseEventArgs& e)
		{
			super::OnMouseWheel(e);

			if (!GetEnabled() || !GetVisible())
				return;

			int32 oldValue = GetValue();
			SetThumbValue(oldValue + GetSingleStep() * -e.Delta());

			UpdateLayout();

			if (oldValue != GetValue())
			{
				ScrollEventType scrollEventType;
				if (e.Delta() < 0)
					scrollEventType = ScrollEventType_SingleDecrement;
				else
					scrollEventType = ScrollEventType_SingleIncrement;

				Scroll.Invoke(this, ScrollEventArgs(scrollEventType,
					oldValue, GetValue(), GetOrientation()));
			}
		}

		void ScrollBar::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				UISystem::Instance()->SetCapture(this);

				int32 oldValue = GetValue();
				int32 scrollEventType = -1;
				PointInt mouse = GlobalToLocal(e.Location());

				if (GetOrientation() == Orientation_Horizontal)
				{
					if (mouse.X < _ThumbButton->GetLeft())
					{
						scrollEventType = ScrollEventType_PageDecrement;
						SetThumbValue(oldValue - GetPageStep());
					}
					else if (mouse.X > _ThumbButton->GetRight())
					{
						scrollEventType = ScrollEventType_PageIncrement;
						SetThumbValue(oldValue + GetPageStep());
					}
				}
				else// if (GetOrientation() == Orientation_Vertical)
				{
					if (mouse.Y < _ThumbButton->GetTop())
					{
						scrollEventType = ScrollEventType_PageDecrement;
						SetThumbValue(oldValue - GetPageStep());
					}
					else if (mouse.Y > _ThumbButton->GetBottom())
					{
						scrollEventType = ScrollEventType_PageIncrement;
						SetThumbValue(oldValue + GetPageStep());
					}
				}

				if (scrollEventType != -1 && oldValue != GetValue())
				{
					UpdateLayout();

					Scroll.Invoke(this, ScrollEventArgs((ScrollEventType)scrollEventType,
						oldValue, GetValue(), GetOrientation()));
				}
			}
		}

		void ScrollBar::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				UISystem::Instance()->ReleaseCapture();
			}
		}

		void ScrollBar::OnKeyDown(KeyEventArgs& e)
		{
			super::OnKeyDown(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (GetOrientation() == Orientation_Horizontal)
			{
				if (e.Key() == Input::Key_Left)
				{
					SetThumbValue(GetValue() - GetSingleStep());
				}
				else if (e.Key() == Input::Key_Right)
				{
					SetThumbValue(GetValue() + GetSingleStep());
				}
			}
			else// if (GetOrientation() == Orientation_Vertical)
			{
				if (e.Key() == Input::Key_Up)
				{
					SetThumbValue(GetValue() - GetSingleStep());
				}
				else if (e.Key() == Input::Key_Down)
				{
					SetThumbValue(GetValue() + GetSingleStep());
				}
			}
		}


		ScrollBar::ScrollBarButton::ScrollBarButton() :
			PushButton(),
			_ButtonType(ScrollButtonType_LeftHorizontal)
		{
		}

		ScrollBar::ScrollBarButton::~ScrollBarButton()
		{
		}

		void ScrollBar::ScrollBarButton::DoRender()
		{
			PushButton::DoRender();

			// Draw arrows
			static const int32 ArrowMargin = 2;
			RectangleInt rect = LocalToGlobal(GetClientRectangle());

			switch (_ButtonType)
			{
			case ScrollButtonType_LeftHorizontal:
				UISystem::Instance()->FillTriangle(GetForegroundColor(),
					PointInt(rect.GetLeft() + 2*ArrowMargin, rect.GetTop() + ArrowMargin +
						(rect.GetHeight() - 2*ArrowMargin) / 2),
					PointInt(rect.GetRight() - 2*ArrowMargin, rect.GetTop() + ArrowMargin),
					PointInt(rect.GetRight() - 2*ArrowMargin, rect.GetBottom() - ArrowMargin));
				break;

			case ScrollButtonType_RightHorizontal:
				UISystem::Instance()->FillTriangle(GetForegroundColor(),
					PointInt(rect.GetRight() - 2*ArrowMargin, rect.GetTop() + ArrowMargin +
						(rect.GetHeight() - 2*ArrowMargin) / 2),
					PointInt(rect.GetLeft() + 2*ArrowMargin, rect.GetTop() + ArrowMargin),
					PointInt(rect.GetLeft() + 2*ArrowMargin, rect.GetBottom() - ArrowMargin));
				break;

			case ScrollButtonType_TopVertical:
				UISystem::Instance()->FillTriangle(GetForegroundColor(),
					PointInt(rect.GetLeft() + ArrowMargin +
						(rect.GetWidth() - 2*ArrowMargin) / 2, rect.GetTop() + 2*ArrowMargin),
					PointInt(rect.GetLeft() + ArrowMargin, rect.GetBottom() - 2*ArrowMargin),
					PointInt(rect.GetRight() - ArrowMargin, rect.GetBottom() - 2*ArrowMargin));
				break;

			case ScrollButtonType_BottomVertical:
				UISystem::Instance()->FillTriangle(GetForegroundColor(),
					PointInt(rect.GetLeft() + ArrowMargin +
						(rect.GetWidth() - 2*ArrowMargin) / 2, rect.GetBottom() - 2*ArrowMargin),
					PointInt(rect.GetLeft() + ArrowMargin, rect.GetTop() + 2*ArrowMargin),
					PointInt(rect.GetRight() - ArrowMargin, rect.GetTop() + 2*ArrowMargin));
				break;
			}
		}
	}
}

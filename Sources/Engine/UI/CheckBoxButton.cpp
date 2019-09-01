/*=============================================================================
CheckBoxButton.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "CheckBoxButton.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(CheckBoxButton);
		SE_IMPLEMENT_REFLECTION(CheckBoxButton);

		CheckBoxButton::CheckBoxButton() :
			super()
		{
			_TextAlignment = Alignment_MiddleLeft;
			_PushedColor = _BackgroundColor;

			_Checked = false;
			_CheckAlignment = Alignment_MiddleLeft;
			_CheckedColor = _BackgroundColor;
		}

		CheckBoxButton::~CheckBoxButton()
		{
		}

		void CheckBoxButton::SetChecked(bool value)
		{
			_Checked = value;
			CheckedChanged.Invoke(this, EventArgs::Empty);
		}

		void CheckBoxButton::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void CheckBoxButton::DoRender()
		{
			RectangleInt rect = LocalToGlobal(GetClientRectangle());
			RectangleInt textRect = LocalToGlobal(GetClientRectangle());

			Color32 color = GetBackgroundColor();
			if (GetChecked())
			{
				color = GetCheckedColor();
			}
			if (GetPushed())
			{
				color = GetPushedColor();
			}
			else if (0)
			{
				color = GetHoverColor();
			}

			if (GetHasFrame())
			{
				RectangleInt frameRect = LocalToGlobal(RectangleInt(PointInt(0, 0), GetSize()));
				UISystem::Instance()->DrawRectangle(GetFrameColor(), GetFrameSize(), frameRect);
			}

			if (GetHasBackground())
			{
				RectangleInt bgRect = LocalToGlobal(GetClientRectangle());
				UISystem::Instance()->FillRectangle(color, bgRect);
			}

			RectangleInt checkRect = LocalToGlobal(GetClientRectangle());
			Vector2 position = UISystem::GetPositionFromAlignment(SizeInt(11, 11), checkRect, _CheckAlignment);
			checkRect.SetLocation(PointInt(position.X, position.Y));
			checkRect.SetSize(SizeInt(11, 11));

			if (GetChecked())
			{
				UISystem::Instance()->FillRectangle(Color32::White, checkRect);
				UISystem::Instance()->DrawRectangle(Color32::Black, 1, checkRect);

				checkRect.X += 3;
				checkRect.Y += 3;
				checkRect.Width -= 6;
				checkRect.Height -= 6;
				UISystem::Instance()->FillRectangle(Color32::Black, checkRect);
			}
			else
			{
				UISystem::Instance()->FillRectangle(Color32::White, checkRect);
				UISystem::Instance()->DrawRectangle(Color32::Black, 1, checkRect);
			}

			textRect.X += 11 + 6;

			if (!GetText().IsEmpty() && GetFont() != NULL)
			{
				_TextObj->SetText(GetText());
				_TextObj->SetFont(GetFont());
				_TextObj->SetColor(GetForegroundColor());
				_TextObj->SetFormat(UISystem::Instance()->GetTextFormat(GetTextAlignment()));
				_TextObj->SetBounds(RectangleReal(textRect.X, textRect.Y, textRect.Width, textRect.Height));
				_TextObj->Render();
			}
		}

		void CheckBoxButton::OnMouseEnter(EventArgs& e)
		{
			super::OnMouseEnter(e);
		}

		void CheckBoxButton::OnMouseLeave(EventArgs& e)
		{
			super::OnMouseLeave(e);
		}

		void CheckBoxButton::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);
		}

		void CheckBoxButton::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				SetChecked(!GetChecked());
			}
		}

		void CheckBoxButton::OnKeyDown(KeyEventArgs& e)
		{
			super::OnKeyDown(e);
		}

		void CheckBoxButton::OnKeyUp(KeyEventArgs& e)
		{
			super::OnKeyUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Key() == Input::Key_Space)
			{
				SetChecked(!GetChecked());
			}
		}
	}
}

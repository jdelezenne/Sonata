/*=============================================================================
RadioButton.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RadioButton.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(RadioButton);
		SE_IMPLEMENT_REFLECTION(RadioButton);

		RadioButton::RadioButton() :
			super()
		{
			_TextAlignment = Alignment_MiddleLeft;
			_PushedColor = _BackgroundColor;

			_Checked = false;
			_ButtonGroup = 0;
			_CheckAlignment = Alignment_MiddleLeft;

			_CheckedColor = _BackgroundColor;
		}

		RadioButton::~RadioButton()
		{
		}

		void RadioButton::SetChecked(bool value)
		{
			_Checked = value;
			CheckedChanged.Invoke(this, EventArgs::Empty);
		}

		void RadioButton::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void RadioButton::DoRender()
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
			Vector2 position = UISystem::GetPositionFromAlignment(SizeInt(12, 12), checkRect, _CheckAlignment);
			checkRect.SetLocation(PointInt(position.X, position.Y));
			checkRect.SetSize(SizeInt(12, 12));

			PointInt center = PointInt(checkRect.GetLocation().X + 6, checkRect.GetLocation().Y + 6);
			if (GetChecked())
			{
				UISystem::Instance()->FillCircle(Color32::White, center, 6);
				UISystem::Instance()->DrawCircle(Color32::Black, 1, center, 6);
				UISystem::Instance()->FillCircle(Color32::Black, center, 3);
			}
			else
			{
				UISystem::Instance()->FillCircle(Color32::White, center, 6);
				UISystem::Instance()->DrawCircle(Color32::Black, 1, center, 6);
			}

			textRect.X += 12 + 6;

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

		void RadioButton::OnMouseEnter(EventArgs& e)
		{
			super::OnMouseEnter(e);
		}

		void RadioButton::OnMouseLeave(EventArgs& e)
		{
			super::OnMouseLeave(e);
		}

		void RadioButton::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);
		}

		void RadioButton::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;

			if (e.Button() == MouseButtons_Left)
			{
				SetChecked(!GetChecked());
			}
		}

		void RadioButton::OnKeyDown(KeyEventArgs& e)
		{
			super::OnKeyDown(e);
		}

		void RadioButton::OnKeyUp(KeyEventArgs& e)
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

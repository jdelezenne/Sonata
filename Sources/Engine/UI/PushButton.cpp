/*=============================================================================
PushButton.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PushButton.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(PushButton);
		SE_IMPLEMENT_REFLECTION(PushButton);

		PushButton::PushButton() :
			super()
		{
			_Rectangle.Height = UISystem::Instance()->DefaultSize;
			_HasFrame = true;
			_FrameSize = 1;
			_TextAlignment = Alignment_MiddleCenter;
		}

		PushButton::~PushButton()
		{
		}

		void PushButton::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void PushButton::DoRender()
		{
			RectangleInt rect = LocalToGlobal(GetClientRectangle());
			RectangleInt textRect = LocalToGlobal(GetClientRectangle());

			Color32 color = GetBackgroundColor();
			if (GetPushed())
			{
				color = GetPushedColor();
			}
			else if (_MouseHover || UISystem::Instance()->GetCapture() == this)
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
	}
}

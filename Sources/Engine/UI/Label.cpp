/*=============================================================================
Label.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Label.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Label);
		SE_IMPLEMENT_REFLECTION(Label);

		Label::Label() :
			Widget()
		{
			_Transparent = true;

			_TextAlignment = Alignment_TopLeft;
			_TextObj = new Text();
		}

		Label::~Label()
		{
		}

		void Label::DoUpdate(real64 elapsed)
		{
			Widget::DoUpdate(elapsed);
		}

		void Label::DoRender()
		{
			Widget::DoRender();

			if (!GetText().IsEmpty() && GetFont() != NULL)
			{
				RectangleInt textRect = LocalToGlobal(GetClientRectangle());

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

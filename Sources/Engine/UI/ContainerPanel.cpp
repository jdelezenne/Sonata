/*=============================================================================
ContainerPanel.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ContainerPanel.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(ContainerPanel);
		SE_IMPLEMENT_REFLECTION(ContainerPanel);

		ContainerPanel::ContainerPanel() :
			super()
		{
		}

		ContainerPanel::~ContainerPanel()
		{
		}

		void ContainerPanel::UpdateScrollbars()
		{
			RectangleInt childRect;

			WidgetList::Iterator it = _Children->GetIterator();
			if (it.Next())
			{
				childRect = it.Current()->GetRectangle();
			}

			while (it.Next())
			{
				childRect.Union(it.Current()->GetRectangle());
			}

			if (_HorizontalScrollState)
				childRect.Width += UISystem::DefaultSize;

			if (_VerticalScrollState)
				childRect.Height += UISystem::DefaultSize;

			_HorizontalScrollBar->SetMinimum(childRect.X);
			_HorizontalScrollBar->SetMaximum(childRect.Width);
			_VerticalScrollBar->SetMinimum(childRect.Y);
			_VerticalScrollBar->SetMaximum(childRect.Height);
		}

		void ContainerPanel::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void ContainerPanel::DoRender()
		{
			super::DoRender();
		}
	}
}

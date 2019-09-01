/*=============================================================================
ScrollablePanel.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ScrollablePanel.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(ScrollablePanel);
		SE_IMPLEMENT_REFLECTION(ScrollablePanel);

		ScrollablePanel::ScrollablePanel() :
			super()
		{
			_HorizontalScrollState = false;
			_VerticalScrollState = false;

			_HorizontalScrollBar = new ScrollBar();
			_VerticalScrollBar = new ScrollBar();

			Children().Add(_HorizontalScrollBar);
			Children().Add(_VerticalScrollBar);

			_HorizontalScrollBar->SetOrientation(Orientation_Horizontal);
			_VerticalScrollBar->SetOrientation(Orientation_Vertical);
		}

		ScrollablePanel::~ScrollablePanel()
		{
			//SE_DELETE(_HorizontalScrollBar);
			//SE_DELETE(_VerticalScrollBar);
		}

		void ScrollablePanel::UpdateLayout()
		{
			_HorizontalScrollBar->SetVisible(_HorizontalScrollState);
			_VerticalScrollBar->SetVisible(_VerticalScrollState);

			_HorizontalScrollBar->SetPosition(PointInt(0, GetHeight() - _HorizontalScrollBar->GetHeight()));
			_HorizontalScrollBar->SetWidth(GetWidth() - _VerticalScrollBar->GetWidth());

			_VerticalScrollBar->SetPosition(PointInt(GetWidth() - _VerticalScrollBar->GetWidth(), 0));
			_VerticalScrollBar->SetHeight(GetHeight() - _HorizontalScrollBar->GetHeight());
		}

		void ScrollablePanel::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void ScrollablePanel::DoRender()
		{
			super::DoRender();
		}

		void ScrollablePanel::OnSize(EventArgs& e)
		{
			super::OnSize(e);

			UpdateLayout();
		}

		void ScrollablePanel::OnMouseMove(MouseEventArgs& e)
		{
			super::OnMouseMove(e);
		}

		void ScrollablePanel::OnMouseDown(MouseEventArgs& e)
		{
			super::OnMouseDown(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}

		void ScrollablePanel::OnMouseUp(MouseEventArgs& e)
		{
			super::OnMouseUp(e);

			if (!GetEnabled() || !GetVisible())
				return;
		}
	}
}

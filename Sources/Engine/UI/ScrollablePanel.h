/*=============================================================================
ScrollablePanel.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_SCROLLABLEPANEL_H_
#define _SE_UI_SCROLLABLEPANEL_H_

#include "UI/Common.h"
#include "UI/Panel.h"
#include "UI/ScrollBar.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Base class for the scrollable panels.
		*/
		class SE_UI_EXPORT ScrollablePanel : public Panel
		{
			SE_DECLARE_CLASS(ScrollablePanel, Panel);

			SE_BEGIN_REFLECTION(ScrollablePanel);
			SE_END_REFLECTION(ScrollablePanel);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ScrollablePanel();

			/** Destructor. */
			virtual ~ScrollablePanel();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets a value indicating the state of the horizontal scroll bar. */
			bool GetHorizontalScrollState() const { return _HorizontalScrollState; }
			void SetHorizontalScrollState(bool value) { _HorizontalScrollState = value; }

			/** Gets or sets a value indicating the state of the vertical scroll bar. */
			bool GetVerticalScrollState() const { return _VerticalScrollState; }
			void SetVerticalScrollState(bool value) { _VerticalScrollState = value; }

			/** Gets or sets the horizontal scroll bar. */
			ScrollBar* GetHorizontalScrollBar() const { return _HorizontalScrollBar; }
			void SetHorizontalScrollBar(ScrollBar* value) { _HorizontalScrollBar = value; }

			/** Gets or sets the vertical scroll bar. */
			ScrollBar* GetVerticalScrollBar() const { return _VerticalScrollBar; }
			void SetVerticalScrollBar(ScrollBar* value) { _VerticalScrollBar = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnSize(EventArgs& e);
			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the client area is scrolled. */
			DefaultEvent Scroll;
			//@}

		protected:
			void UpdateLayout();

		protected:
			bool _HorizontalScrollState;
			bool _VerticalScrollState;
			ScrollBar* _HorizontalScrollBar;
			ScrollBar* _VerticalScrollBar;
		};
	}
}

#endif 

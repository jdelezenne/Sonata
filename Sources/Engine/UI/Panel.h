/*=============================================================================
Panel.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_PANEL_H_
#define _SE_UI_PANEL_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Used to group collections of widgets.
		*/
		class SE_UI_EXPORT Panel : public Widget
		{
			SE_DECLARE_CLASS(Panel, Widget);

			SE_BEGIN_REFLECTION(Panel);
				SE_Field(_Movable, bool, Public);
				SE_Field(_Sizable, bool, Public);
			SE_END_REFLECTION(Panel);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Panel();

			/** Destructor. */
			virtual ~Panel();
			//@}

			/** @name Properties. */
			//@{
			bool GetMovable() const { return _Movable; }
			void SetMovable(bool value) { _Movable = value; }

			bool GetSizable() const { return _Sizable; }
			void SetSizable(bool value) { _Sizable = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnMouseMove(MouseEventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			bool _Movable;
			bool _Sizable;

			bool _Moving;
			FrameLocation _SizingFrame;
			PointInt _MovingOrigin;
			SizeInt _SizingOrigin;
		};
	}
}

#endif 

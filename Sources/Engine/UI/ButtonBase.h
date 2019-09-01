/*=============================================================================
ButtonBase.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_BUTTONBASE_H_
#define _SE_UI_BUTTONBASE_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Base class for button widgets.
		*/
		class SE_UI_EXPORT ButtonBase : public Widget
		{
			SE_DECLARE_ABSTRACT(ButtonBase, Widget);

			SE_BEGIN_REFLECTION(ButtonBase);
				SE_Field(_Pushed, bool, Public);
				SE_Field(_Text, String, Public);
				SE_Field(_TextAlignment, Alignment, Public);
				SE_Field(_PushedColor, Color32, Public);
				SE_Field(_HoverColor, Color32, Public);
			SE_END_REFLECTION(ButtonBase);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ButtonBase();

			/** Destructor. */
			virtual ~ButtonBase();
			//@}

			/** @name Properties. */
			//@{
			/** Gets state of the button. */
			bool GetPushed() const { return _Pushed; }

			/** Gets or sets the current text in the widget. */
			String GetText() const { return _Text; }
			void SetText(const String& value) { _Text = value; }

			/** Gets or sets the alignment of the text in the button. */
			Alignment GetTextAlignment() const { return _TextAlignment; }
			void SetTextAlignment(Alignment value) { _TextAlignment = value; }

			Color32 GetPushedColor() const { return _PushedColor; }
			void SetPushedColor(const Color32& value) { _PushedColor = value; }

			Color32 GetHoverColor() const { return _HoverColor; }
			void SetHoverColor(const Color32& value) { _HoverColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();

			virtual void OnMouseEnter(EventArgs& e);
			virtual void OnMouseLeave(EventArgs& e);
			virtual void OnMouseDown(MouseEventArgs& e);
			virtual void OnMouseUp(MouseEventArgs& e);
			virtual void OnKeyDown(KeyEventArgs& e);
			virtual void OnKeyUp(KeyEventArgs& e);
			//@}

			/** @name Events. */
			//@{
			/** Occurs when the button is clicked. */
			DefaultEvent Click;
			//@}

		protected:
			bool _Pushed;
			String _Text;
			Alignment _TextAlignment;

			Color32 _PushedColor;
			Color32 _HoverColor;

			TextPtr _TextObj;
			bool _MouseHover;
		};
	}
}

#endif 

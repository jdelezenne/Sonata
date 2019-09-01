/*=============================================================================
Label.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_LABEL_H_
#define _SE_UI_LABEL_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Provides text display.
		*/
		class SE_UI_EXPORT Label : public Widget
		{
			SE_DECLARE_CLASS(Label, Widget);

			SE_BEGIN_REFLECTION(Label);
				SE_Field(_Text, String, Public);
				SE_Field(_TextAlignment, Alignment, Public);
			SE_END_REFLECTION(Label);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Label();

			/** Destructor. */
			virtual ~Label();
			//@}

			/** @name Properties. */
			//@{
			/** Gets or sets the current text in the widget. */
			String GetText() const { return _Text; }
			void SetText(const String& value) { _Text = value; }

			/** Gets or sets the alignment of the text in the label. */
			Alignment GetTextAlignment() const { return _TextAlignment; }
			void SetTextAlignment(Alignment value) { _TextAlignment = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			String _Text;
			Alignment _TextAlignment;

			TextPtr _TextObj;
		};
	}
}

#endif 

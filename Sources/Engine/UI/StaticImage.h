/*=============================================================================
StaticImage.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_IMAGE_H_
#define _SE_UI_IMAGE_H_

#include "UI/Common.h"
#include "UI/Widget.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Provides image display.
		*/
		class SE_UI_EXPORT StaticImage : public Widget
		{
			SE_DECLARE_CLASS(StaticImage, Widget);

			SE_BEGIN_REFLECTION(StaticImage);
				SE_Field(_ImageSource, String, Public);
				SE_Field(_Alignment, Alignment, Public);
				SE_Field(_ImageLayout, ImageLayout, Public);
				//SE_Field(_AlphaState, AlphaState, Public);
			SE_END_REFLECTION(StaticImage);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			StaticImage();

			/** Destructor. */
			virtual ~StaticImage();
			//@}

			virtual void OnDeserialized(BinarySerializer* context, BinaryStream& stream);
			virtual void OnDeserialized(XMLSerializer* context, XMLElement* element);

			/** @name Properties. */
			//@{
			/** Gets or sets the image that is displayed on the image widget. */
			Texture* GetImage() const { return _Image; }
			void SetImage(Texture* value) { _Image = value; }

			Sprite* GetSprite() const { return _Sprite; }
			void SetSprite(Sprite* value) { _Sprite = value; }

			bool GetUseSprite() const { return _UseSprite; }
			void SetUseSprite(bool value) { _UseSprite = value; }

			/** Gets or sets the alignment of the image in the image widget. */
			Alignment GetAlignment() const { return _Alignment; }
			void SetAlignment(Alignment value) { _Alignment = value; }

			/** Gets or sets the layout of the image in the image widget. */
			ImageLayout GetImageLayout() const { return _ImageLayout; }
			void SetImageLayout(ImageLayout value) { _ImageLayout = value; }

			AlphaState GetAlphaState() const { return _AlphaState; }
			void SetAlphaState(AlphaState value) { _AlphaState = value; }
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
			String _ImageSource;
			TexturePtr _Image;
			SpritePtr _Sprite;
			bool _UseSprite;
			Alignment _Alignment;
			ImageLayout _ImageLayout;
			AlphaState _AlphaState;
		};
	}
}

#endif 

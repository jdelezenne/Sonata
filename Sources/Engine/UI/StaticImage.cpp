/*=============================================================================
StaticImage.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "StaticImage.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(StaticImage);
		SE_IMPLEMENT_REFLECTION(StaticImage);

		StaticImage::StaticImage() :
			Widget()
		{
			_Image = NULL;
			_Sprite = NULL;
			_UseSprite = false;
			_Alignment = Alignment_MiddleCenter;
			_ImageLayout = ImageLayout_Stretch;
		}

		StaticImage::~StaticImage()
		{
		}

		void StaticImage::OnDeserialized(BinarySerializer* context, BinaryStream& stream)
		{
			super::OnDeserialized(context, stream);

			if (!_ImageSource.IsEmpty())
			{
				Resource* resource;

				resource = ResourceHelper::LoadFromFile(_ImageSource, SE_ID_DATA_IMAGE);
				if (resource == NULL)
					return;

				Image* image = (Image*)resource->GetData();
				Texture* texture;
				if (RenderSystem::Current()->CreateTexture(&texture) &&
					texture->Create(image, TextureUsage_Static))
				{
					_Image = texture;
				}
			}
		}

		void StaticImage::OnDeserialized(XMLSerializer* context, XMLElement* element)
		{
			super::OnDeserialized(context, element);

			if (!_ImageSource.IsEmpty())
			{
				Resource* resource;

				resource = ResourceHelper::LoadFromFile(_ImageSource, SE_ID_DATA_IMAGE);
				if (resource == NULL)
					return;

				Image* image = (Image*)resource->GetData();
				Texture* texture;
				if (RenderSystem::Current()->CreateTexture(&texture) &&
					texture->Create(image, TextureUsage_Static))
				{
					_Image = texture;
				}

				_AlphaState.BlendEnable[0] = true;
				_AlphaState.SourceBlend = BlendMode_SourceAlpha;
				_AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;
			}
		}

		void StaticImage::DoUpdate(real64 elapsed)
		{
			Widget::DoUpdate(elapsed);
		}

		void StaticImage::DoRender()
		{
			Widget::DoRender();

			RectangleInt imageRect = LocalToGlobal(GetClientRectangle());
			if (!_UseSprite)
			{
				if (GetImage() != NULL)
				{
					UISystem::Instance()->DrawImage(GetImage(), imageRect,
						GetAlignment(), GetImageLayout(), &GetAlphaState());
				}
			}
			else
			{
				if (GetSprite() != NULL)
				{
					_Sprite->SetPosition(Vector2(imageRect.X, imageRect.Y));
					_Sprite->SetSize(Vector2(imageRect.Width, imageRect.Height));
					_Sprite->Render();
				}
			}
		}
	}
}

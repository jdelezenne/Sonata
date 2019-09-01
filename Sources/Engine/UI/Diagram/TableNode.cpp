/*=============================================================================
TableNode.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TableNode.h"
#include "UI/UISystem.h"
#include "UI/Diagram/Connection.h"
#include "UI/Diagram/Diagram.h"
#include "UI/Diagram/Link.h"
#include "UI/Diagram/Slot.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(TableProperty);
		SE_IMPLEMENT_REFLECTION(TableProperty);

		TableProperty::TableProperty() :
			super()
		{
			_Owner = NULL;
			_PropertyType = TablePropertyType_Input;

			Slot* slot = new Slot();
			slot->SetParent(this);
			slot->SetSize(SizeInt(2, 2));
			slot->SetBackgroundColor(Color32::Blue);
			Slots().Add(slot);

			_TextObj = new Text();
		}

		TableProperty::~TableProperty()
		{
		}

		void TableProperty::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);

			SizeReal sz = Text::MeasureText(GetPropertyName(), GetFont());
			SetMinimumSize(SizeInt(sz.Width, sz.Height));
		}

		void TableProperty::DoRender()
		{
			super::DoRender();

			if (!GetPropertyName().IsEmpty() && GetFont() != NULL)
			{
				RectangleInt textRect = LocalToGlobal(GetClientRectangle());

				_TextObj->SetText(GetPropertyName());
				_TextObj->SetFont(GetFont());
				_TextObj->SetColor(GetForegroundColor());
				_TextObj->SetFormat(UISystem::Instance()->GetTextFormat(Alignment_MiddleLeft));
				_TextObj->SetBounds(RectangleReal(textRect.X, textRect.Y, textRect.Width, textRect.Height));
				_TextObj->Render();
			}
		}

		TablePreview::TablePreview(TableNode* owner) :
			_Owner(owner)
		{
			_PreviewType = TablePreviewType_Text;
			_Image = NULL;

			_TextObj = new Text();
			_texture = NULL;
		}

		TablePreview::~TablePreview()
		{
		}

		int32 TablePreview::GetWidth() const
		{
			return _Size.Width;
		}

		int32 TablePreview::GetHeight() const
		{
			return _Size.Height;
		}

		void TablePreview::SetImage(Image* value)
		{
			if (_Image != value)
			{
				SE_DELETE(_texture);
				_Image = value;
			}
		}

		void TablePreview::Update(real64 elapsed)
		{
			_Size = SizeInt(0, 0);

			switch (_PreviewType)
			{
			case TablePreviewType_Text:
				{
				SizeReal sz = Text::MeasureText(GetText(), _Owner->GetFont());
				_Size = SizeInt(sz.Width * 2, sz.Height * 2);
				}
				break;

			case TablePreviewType_Image:
				if (_Image != NULL)
				{
					_Size = SizeInt(_Image->GetWidth(), _Image->GetHeight());

					if (RenderSystem::Current()->CreateTexture(&_texture))
					{
						_texture->Create(_Image, TextureUsage_Static);
					}
				}
				break;
			}
		}

		void TablePreview::Render()
		{
			RectangleInt rect = RectangleInt(_Position, _Size);

			switch (_PreviewType)
			{
			case TablePreviewType_Text:
				if (!GetText().IsEmpty() && _Owner->GetFont() != NULL)
				{
					_TextObj->SetText(GetText());
					_TextObj->SetFont(_Owner->GetFont());
					_TextObj->SetColor(Color32::Black);
					//_TextObj->SetFormat(UISystem::Instance()->GetTextFormat(Alignment_TopCenter));
					_TextObj->SetFormat(UISystem::Instance()->GetTextFormat(Alignment_MiddleLeft));
					_TextObj->SetScale(2.0f);
					_TextObj->SetBounds(RectangleReal(rect.X, rect.Y, rect.Width, rect.Height));
					_TextObj->Render();
				}
				break;

			case TablePreviewType_Image:
				if (_texture != NULL)
				{
					UISystem::Instance()->DrawImage(_texture, rect,
						Alignment_TopCenter, ImageLayout_Stretch);
				}
				break;
			}
		}

		SE_IMPLEMENT_CLASS(TableNode);
		SE_IMPLEMENT_REFLECTION(TableNode);

		TableNode::TableNode() :
			super()
		{
			_HasFrame = true;
			_FrameSize = 1;
			_Movable = true;

			_ShowTitle = true;
			_ShowPreview = true;
			_ShowProperties = true;
			_Preview = NULL;

			_TextObj = new Text();
		}

		TableNode::~TableNode()
		{
		}

		void TableNode::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);

			int i;
			int32 width = 0;
			int32 height = 0;

			int32 titleWidth = 0;
			if (_ShowTitle)
			{
				titleWidth = Text::MeasureText(GetTitle(), GetFont()).Width;
				height += GetFont()->GetHeight();
			}

			int32 previewWidth = 0;
			if (_ShowPreview && _Preview != NULL)
			{
				_Preview->Update(elapsed);

				_Preview->SetPosition(LocalToGlobal(PointInt(0, height)));
				previewWidth = _Preview->GetWidth();
				height += _Preview->GetHeight();
			}

			int32 propertiesWidth = 0;
			if (_ShowProperties)
			{
				int32 inputCount = _InputProperties.Count();
				int32 outputCount = _OutputProperties.Count();

				int32 maxInputWidth = 0;
				for (i=0; i<inputCount; i++)
				{
					TableProperty* property = _InputProperties[i];
					if (!Children().Contains(property))
					{
						property->SetOwner(this);
						property->SetPropertyType(TablePropertyType_Input);
						AddNode(property);
					}

					property->SetPosition(PointInt(0, height + i*GetFont()->GetHeight()));
					//property->SetPosition(LocalToGlobal(PointInt(0, height + i*GetFont()->GetHeight())));
					//property->Update(elapsed);
					maxInputWidth = Math::Max(maxInputWidth, property->GetMinimumSize().Width);
				}

				for (i=0; i<inputCount; i++)
				{
					_InputProperties[i]->SetWidth(maxInputWidth);
					_InputProperties[i]->Slots().GetItem(0)->SetPosition(
						LocalToGlobal(PointInt(0, height +
						i*GetFont()->GetHeight() + GetFont()->GetHeight() / 2)));
				}

				int32 maxOutputWidth = 0;
				for (i=0; i<outputCount; i++)
				{
					TableProperty* property = _OutputProperties[i];
					if (!Children().Contains(property))
					{
						property->SetOwner(this);
						property->SetPropertyType(TablePropertyType_Output);
						AddNode(property);
					}

					property->SetPosition(PointInt(maxInputWidth + 10, height + i*GetFont()->GetHeight()));
					//property->SetPosition(LocalToGlobal(PointInt(maxInputWidth, height + i*GetFont()->GetHeight())));
					//property->Update(elapsed);
					maxOutputWidth = Math::Max(maxOutputWidth, property->GetMinimumSize().Width);
				}

				for (i=0; i<outputCount; i++)
				{
					_OutputProperties[i]->SetWidth(maxOutputWidth);
					_OutputProperties[i]->Slots().GetItem(0)->SetPosition(
						LocalToGlobal(PointInt(maxInputWidth + 10 + maxOutputWidth, height +
						i*GetFont()->GetHeight() + GetFont()->GetHeight() / 2)));
				}

				propertiesWidth = maxInputWidth + 10 + maxOutputWidth;
				height += Math::Max(inputCount, outputCount) * GetFont()->GetHeight();
			}

			width = Math::Max(titleWidth, Math::Max(previewWidth, propertiesWidth));
			SetSize(SizeInt(width, height));
		}

		void TableNode::DoRender()
		{
			super::DoRender();

			if (_ShowTitle && !GetTitle().IsEmpty() && GetFont() != NULL)
			{
				RectangleInt textRect = LocalToGlobal(GetClientRectangle());
				textRect.Height = GetFont()->GetHeight();

				_TextObj->SetText(GetTitle());
				_TextObj->SetFont(GetFont());
				_TextObj->SetColor(GetForegroundColor());
				_TextObj->SetFormat(UISystem::Instance()->GetTextFormat(Alignment_TopCenter));
				_TextObj->SetBounds(RectangleReal(textRect.X, textRect.Y, textRect.Width, textRect.Height));
				_TextObj->Render();
			}

			if (_ShowPreview && _Preview != NULL)
			{
				_Preview->Render();
			}

			if (_ShowProperties)
			{
				/*TablePropertyList::Iterator inIt = _InputProperties.GetIterator();
				while (inIt.Next())
				{
					TableProperty* property = inIt.Current();
					property->Render();
				}

				TablePropertyList::Iterator outIt = _OutputProperties.GetIterator();
				while (outIt.Next())
				{
					TableProperty* property = outIt.Current();
					property->Render();
				}*/
			}
		}
	}
}

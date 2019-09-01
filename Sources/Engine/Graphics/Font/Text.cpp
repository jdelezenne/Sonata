/*=============================================================================
Text.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Text.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"

namespace SonataEngine
{

Text::Text() :
	RefObject(),
	_Font(NULL),
	_Scale(1.0f),
	_NeedUpdateMesh(true),
	_NeedUpdateColor(true),
	_mesh(NULL)
{
	CreateMesh();
}

Text::~Text()
{
}

void Text::SetText(const String& value)
{
	if (_Text.CompareTo(value, false) != 0)
	{
		_Text = value;
		_NeedUpdateMesh = true;
	}
}

void Text::SetColor(const Color32& value)
{
	if (_Color != value)
	{
		_Color = value;
		_NeedUpdateColor = true;
	}
}

void Text::CreateMesh()
{
	RenderSystem* renderer = RenderSystem::Current();

	SE_DELETE(_vertexLayout);

	VertexLayout* vertexLayout = NULL;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		SEthrow(Exception());
		return;
	}

	_vertexLayout = vertexLayout;

	uint16 offset = 0;
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3,
		VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2,
		VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color,
		VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	MeshPart* meshPart = new MeshPart();
	meshPart->SetIndexed(true);
	meshPart->SetPrimitiveType(PrimitiveType_TriangleList);

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
}

void Text::UpdateMesh()
{
	RenderSystem* renderer = RenderSystem::Current();

	int32 length = _Text.Length();
	int32 charCount = length;

	SizeReal textSize = Text::MeasureText(_Text, _Font, _Format);

	struct TextGeometry
	{
		TextGeometry() {}
		TextGeometry(const Vector3& position, const Vector2& texcoord, uint32 color) :
			Position(position), TexCoord(texcoord), Color(color) {}

		Vector3 Position;
		Vector2 TexCoord;
		uint32 Color;
	};

	TextGeometry* vertices = new TextGeometry[4*length];

	uint32 color = _Color.ToARGB();
	real tu1, tv1;
	real tu2, tv2;

	Vector2 position = Vector2::Zero;
	Vector2 size;

	int32 width, height;
	width = _Font->GetTexture()->GetWidth();
	height = _Font->GetTexture()->GetHeight();

	int charIndex = 0;
	for (int i = 0; i < length; i++)
	{
		Char c = _Text[i];

		if (!_Font->ContainsGlyth(c))
			c = _Format.DefaultGlyth;

		if (c == Char::Lf)
		{
			if ((_Format.Flags & TextFormatFlags_DirectionVertical) == 0)
				position.Y += _Font->GetHeight() * _Scale;
			else
				position.X += _Font->GetMaxWidth() * _Scale;

			charCount--;
			continue;
		}
		else if (Char::IsWhiteSpace(c) || !_Font->ContainsGlyth(c))
		{
			if ((_Format.Flags & TextFormatFlags_DirectionVertical) == 0)
				position.X += (_Font->GetSpaceWidth() + _Font->GetSpacing()) * _Scale;
			else
				position.Y += (_Font->GetHeight() + _Font->GetSpacing()) * _Scale;

			charCount--;
			continue;
		}

		const FontGlyth& glyth = _Font->GetGlyth(c);

		tu1 = (real)glyth.Rectangle.X / (real)width;
		tv1 = (real)glyth.Rectangle.Y / (real)height;
		tu2 = tu1 + (real)glyth.Rectangle.Width / (real)width;
		tv2 = tv1 + (real)glyth.Rectangle.Height / (real)height;

		size = Vector2(glyth.Rectangle.Width * _Scale, glyth.Rectangle.Height * _Scale);

		vertices[4*charIndex] = TextGeometry(Vector3(position.X, position.Y, 0.0),
			Vector2(tu1, tv1), color);
		vertices[4*charIndex+1] = TextGeometry(Vector3(position.X+size.X, position.Y, 0.0),
			Vector2(tu2, tv1), color);
		vertices[4*charIndex+2] = TextGeometry(Vector3(position.X+size.X, position.Y+size.Y, 0.0),
			Vector2(tu2, tv2), color);
		vertices[4*charIndex+3] = TextGeometry(Vector3(position.X, position.Y+size.Y, 0.0),
			Vector2(tu1, tv2), color);

		if ((_Format.Flags & TextFormatFlags_DirectionVertical) == 0)
			position.X += (glyth.Rectangle.Width + _Font->GetSpacing()) * _Scale;
		else
			position.Y += (glyth.Rectangle.Height + _Font->GetSpacing()) * _Scale;

		charIndex++;
	}

	HardwareBuffer* vertexBuffer;
	uint32 vertexCount = 4*charCount;
	if (!renderer->CreateVertexBuffer(vertexCount * sizeof(TextGeometry),
		HardwareBufferUsage_Dynamic, &vertexBuffer))
	{
		SEthrow(Exception());
	}

	TextGeometry* vbData;
	if (!vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData))
		return;

	Memory::Copy(vbData, vertices, vertexCount * sizeof(TextGeometry));
	vertexBuffer->Unmap();

	delete[] vertices;

	HardwareBuffer* indexBuffer;
	uint32 indexCount = 6*charCount;
	if (!renderer->CreateIndexBuffer(indexCount * sizeof(uint16),
		IndexBufferFormat_Int16, HardwareBufferUsage_Static, &indexBuffer))
	{
		SEthrow(Exception());
	}

	// 3---2
	// | / |
	// 0---1
	uint16* ibData;
	if (!indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData))
		return;

	for (int i = 0; i < charCount; i++)
	{
		ibData[6*i] = 4*i;
		ibData[6*i+1] = 4*i+1;
		ibData[6*i+2] = 4*i+2;
		ibData[6*i+3] = 4*i;
		ibData[6*i+4] = 4*i+2;
		ibData[6*i+5] = 4*i+3;
	}

	indexBuffer->Unmap();

	MeshPart* meshPart = _mesh->GetMeshPart(0);

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = _vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, _vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	meshPart->SetVertexData(vertexData);

	IndexData* indexData = new IndexData();
	indexData->IndexBuffer = indexBuffer;
	indexData->IndexCount = indexCount;
	meshPart->SetIndexData(indexData);

	meshPart->SetPrimitiveCount(indexCount);
}

void Text::UpdateColor()
{
}

void Text::Render()
{
	if (_Font == NULL || _mesh == NULL)
		return;

	if (_Text.IsEmpty())
		return;

	if (_NeedUpdateMesh)
	{
		UpdateMesh();
		_NeedUpdateMesh = false;
		_NeedUpdateColor = false;
	}

	if (_NeedUpdateColor)
	{
		UpdateColor();
		_NeedUpdateColor = false;
	}

	_mesh->GetMeshPart(0)->SetShader(_Font->GetShader());

	Vector2 position = Vector2(_Bounds.GetLocation().X, _Bounds.GetLocation().Y);
	SizeReal textSize = Text::MeasureText(_Text, _Font, _Format);
	if (_Bounds.GetSize().IsEmpty())
	{
		_Bounds.SetSize(textSize);
	}

	if (_Format.HAlignment == HorizontalAlignment_Center)
	{
		if (_Bounds.GetSize().Width > textSize.Width)
			position.X += (_Bounds.GetSize().Width - textSize.Width) / 2;
	}
	else if (_Format.HAlignment == HorizontalAlignment_Right)
	{
		if (_Bounds.GetSize().Width > textSize.Width)
			position.X += _Bounds.GetSize().Width - textSize.Width;
	}

	if (_Format.VAlignment == VerticalAlignment_Center)
	{
		if (_Bounds.GetSize().Height > textSize.Height)
			position.Y += (_Bounds.GetSize().Height - textSize.Height) / 2;
	}
	else if (_Format.VAlignment == VerticalAlignment_Bottom)
	{
		if (_Bounds.GetSize().Height > textSize.Height)
			position.Y += _Bounds.GetSize().Height - textSize.Height;
	}

	Vector3 worldPosition(position.X, position.Y, 0.0);
	SceneManager::Instance()->RenderMesh(_mesh, Matrix4::CreateTranslation(worldPosition));
}

void Text::DrawText(const String& text, Font* font, const PointReal& location, const Color32& color)
{
	Text::DrawText(text, font, RectangleReal(location, SizeReal::Empty), color, TextFormat());
}

void Text::DrawText(const String& text, Font* font, const RectangleReal& bounds, const Color32& color)
{
	Text::DrawText(text, font, bounds, color, TextFormat());
}

void Text::DrawText(const String& text, Font* font, const PointReal& location, const Color32& color, TextFormat format)
{
	Text::DrawText(text, font, RectangleReal(location, SizeReal::Empty), color, format);
}

void Text::DrawText(const String& text, Font* font, const RectangleReal& bounds, const Color32& color, TextFormat format)
{
	static Text* textObj = new Text();

	textObj->SetText(text);
	textObj->SetFont(font);
	textObj->SetBounds(bounds);
	textObj->SetColor(color);
	textObj->SetFormat(format);

	textObj->Render();
}

SizeReal Text::MeasureText(const String& text, Font* font)
{
	return Text::MeasureText(text, font, TextFormat());
}

SizeReal Text::MeasureText(const String& text, Font* font, TextFormat format)
{
	if (text.IsEmpty() || font == NULL)
		return SizeReal::Empty;

	SizeReal size = SizeReal::Empty;
	size.Height = font->GetHeight();

	int length = text.Length();
	for (int i = 0; i < length; i++)
	{
		Char c = text[i];
		if (c == Char::Lf)
		{
			size.Height += font->GetHeight();
		}
		else
		{
			if (!font->ContainsGlyth(c))
				c = format.DefaultGlyth;

			if (font->ContainsGlyth(c))
			{
				FontGlyth glyth = font->GetGlyth(c);
				size.Width += glyth.Rectangle.Width + font->GetSpacing();
			}
			else if (Char::IsWhiteSpace(c))
			{
				size.Width += font->GetSpaceWidth() + font->GetSpacing();
			}
		}
	}

	if ((format.Flags & TextFormatFlags_DirectionVertical) != 0)
	{
		real32 swap = size.Width;
		size.Width = size.Height;
		size.Height = swap;
	}

	return SizeReal(size.Width, size.Height);
}

}

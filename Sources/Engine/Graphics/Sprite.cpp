/*=============================================================================
Sprite.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Sprite.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

SpriteFrame::SpriteFrame() :
	RefObject(),
	_texture(NULL),
	_AnimationDelay(1.0)
{
}

SpriteFrame::~SpriteFrame()
{
}

void SpriteFrame::SetImage(Image* image)
{
	if (image == NULL)
		return;

	RenderSystem* renderer = RenderSystem::Current();
	Texture* texture;
	if (!renderer->CreateTexture(&texture))
		return;

	if (!texture->Create(image, TextureUsage_Static))
	{
		SE_DELETE(texture);
		return;
	}

	_texture = texture;
	_SourceRect = RectangleInt(0, 0, _texture->GetWidth(), _texture->GetHeight());
}

Sprite::Sprite() :
	RefObject(),
	_Invalid(true),
	_Position(Vector2::Zero),
	_Offset(Vector2::Zero),
	_Depth(0.0),
	_Rotation(Vector2::Zero),
	_Size(Vector2::One),
	_Scale(Vector2::One),
	_ImageScale(Vector2::One),
	_Color(Color32::White),
	_currentFrame(0),
	_BackwardAnimation(false),
	_AnimationMode(SpriteAnimationMode_Stop)
{
	AlphaState.SourceBlend = BlendMode_SourceAlpha;
	AlphaState.DestinationBlend = BlendMode_InvSourceAlpha;

	RenderSystem* renderer = RenderSystem::Current();

	VertexLayout* vertexLayout = NULL;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		SEthrow(Exception());
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3,
		VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2,
		VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color,
		VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	MeshPart* meshPart = new MeshPart();

	HardwareBuffer* vertexBuffer;
	uint32 vertexCount = 4;
	if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Dynamic, &vertexBuffer))
	{
		SEthrow(Exception());
	}
	
	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	meshPart->SetVertexData(vertexData);

	HardwareBuffer* indexBuffer;
	uint32 indexCount = 6;
	if (!renderer->CreateIndexBuffer(indexCount * sizeof(uint16),
		IndexBufferFormat_Int16, HardwareBufferUsage_Static, &indexBuffer))
	{
		SEthrow(Exception());
	}

	uint16* ibData;
	if (!indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData))
		return;

	// 3---2
	// | / |
	// 0---1
	uint16 indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Memory::Copy(ibData, indices, 6 * sizeof(uint16));
	indexBuffer->Unmap();

	IndexData* indexData = new IndexData();
	indexData->IndexBuffer = indexBuffer;
	indexData->IndexCount = indexCount;
	meshPart->SetIndexData(indexData);
	meshPart->SetIndexed(true);
	meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, indexCount);

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->RasterizerState.CullMode = CullMode_None;
	pass->DepthState.Enable = false;
	pass->DepthState.WriteEnable = false;
	pass->LightState.Lighting = false;
	SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
	samplerState->MinFilter = TextureFilterType_Linear;
	samplerState->MagFilter = TextureFilterType_Linear;

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
	meshPart->SetShader(shader);

	_UpdateMesh();
}

Sprite::~Sprite()
{
	SpriteFrameList::Iterator it = _SpriteFrames.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}

	SE_DELETE(_mesh);
}

void Sprite::_Invalidate()
{
	_Invalid = true;
}

void Sprite::_UpdateMesh()
{
	struct SpriteGeometry
	{
		Vector3 Position;
		Vector2 TexCoord;
		uint32 Color;
	};

	Vector2 size(_Size.X * _Scale.X, _Size.Y * _Scale.Y);
	uint32 color = _Color.ToARGB();
	real tu1, tv1;
	real tu2, tv2;

	if (_SpriteFrames.Count() == 0)
	{
		tu1 = 0.0;
		tv1 = 0.0;
		tu2 = 1.0;
		tv2 = 1.0;
	}
	else
	{
		SpriteFrame* frame = _SpriteFrames[_currentFrame];
		RectangleInt rect = frame->GetSourceRect();
		int32 width, height;

		width = frame->GetTexture()->GetWidth();
		height = frame->GetTexture()->GetHeight();

		tu1 = (real)rect.X / (real)width;
		tv1 = (real)rect.Y / (real)height;
		tu2 = tu1 + (real)rect.Width / (real)width;
		tv2 = tv1 + (real)rect.Height / (real)height;
	}

	tu2 *= _ImageScale.X;
	tv2 *= _ImageScale.Y;

	HardwareBuffer* vertexBuffer = _mesh->GetMeshPart(0)->GetVertexData()->VertexStreams[0].VertexBuffer;

	SpriteGeometry* vbData;
	if (!vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData))
		return;

	SpriteGeometry vertices[] =
	{
		Vector3(0.0, 0.0, 0.0), Vector2(tu1, tv1), color,
		Vector3(size.X, 0.0, 0.0), Vector2(tu2, tv1), color,
		Vector3(size.X, size.Y, 0.0), Vector2(tu2, tv2), color,
		Vector3(0.0, size.Y, 0.0), Vector2(tu1, tv2), color
	};

	Memory::Copy(vbData, vertices, 4 * sizeof(SpriteGeometry));
	vertexBuffer->Unmap();
}

void Sprite::Update(const TimeValue& timeValue)
{
	if (_Invalid)
	{
		_UpdateMesh();
		_Invalid = false;
	}

	if (_SpriteFrames.Count() < 2)
		return;

	static TimeValue lastTime = timeValue;
	static real64 _DelayRemainder = 0.0;
	_DelayRemainder += (real64)(timeValue - lastTime);
	lastTime = timeValue;

	SpriteFrame* frame = _SpriteFrames[_currentFrame];
	int32 advance = 0;
	advance = _DelayRemainder / frame->GetAnimationDelay();
	_DelayRemainder -= advance * frame->GetAnimationDelay();

	if (advance > 0)
	{
		if (_BackwardAnimation)
		{
			if (_currentFrame > 0)
				_currentFrame--;
			else if (_AnimationMode == SpriteAnimationMode_Loop)
				_currentFrame = _SpriteFrames.Count()-1;
			else if (_AnimationMode == SpriteAnimationMode_Reverse)
			{
				_BackwardAnimation = false;
				_currentFrame++;
			}
		}
		else
		{
			if (_currentFrame < _SpriteFrames.Count()-1)
				_currentFrame++;
			else if (_AnimationMode == SpriteAnimationMode_Loop)
				_currentFrame = 0;
			else if (_AnimationMode == SpriteAnimationMode_Reverse)
			{
				_BackwardAnimation = true;
				_currentFrame--;
			}
		}
	}

	if (_CachedSourceRect != frame->GetSourceRect())
	{
		_UpdateMesh();
		_CachedSourceRect = frame->GetSourceRect();
	}
}

void Sprite::Render()
{
	if (_SpriteFrames.Count() == 0)
		return;

	if (_Invalid)
	{
		_UpdateMesh();
		_Invalid = false;
	}

	RenderSystem* renderer = RenderSystem::Current();
	Matrix4 currentWorld = renderer->GetWorldTransform();

	// Compute the world transform of the sprite
	Vector3 position((_Position.X - _Offset.X) * _Scale.X, (_Position.Y - _Offset.Y) * _Scale.Y, _Depth);

	SpriteFrame* frame = _SpriteFrames[_currentFrame];

	DefaultMaterial* shader = (DefaultMaterial*)_mesh->GetMeshPart(0)->GetShader();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->AlphaState = AlphaState;

	SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
	samplerState->SetTexture(frame->GetTexture());

	SceneManager::Instance()->RenderMesh(_mesh, Matrix4::CreateTranslation(position));

	renderer->SetWorldTransform(currentWorld);
}

int Sprite::GetSpriteFrameCount() const
{
	return _SpriteFrames.Count();
}

bool Sprite::AddSpriteFrame(SpriteFrame* spriteFrame)
{
	if (spriteFrame == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_SpriteFrames.Add(spriteFrame);
		return true;
	}
}

bool Sprite::InsertSpriteFrame(int index, SpriteFrame* spriteFrame)
{
	if (spriteFrame == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_SpriteFrames.Insert(index, spriteFrame);
		return true;
	}
}

bool Sprite::ReplaceSpriteFrame(SpriteFrame* previous, SpriteFrame* spriteFrame)
{
	if (previous == NULL || spriteFrame == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		int index = _SpriteFrames.IndexOf(previous);
		if (index == -1)
		{
			throw "ArgumentException";
			return false;
		}
		else
		{
			_SpriteFrames.SetItem(index, spriteFrame);
			return true;
		}
	}
}

bool Sprite::RemoveSpriteFrame(SpriteFrame* spriteFrame)
{
	if (spriteFrame == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_SpriteFrames.Remove(spriteFrame);
		return true;
	}
}

void Sprite::RemoveAllSpriteFrames()
{
	_SpriteFrames.Clear();
}

int Sprite::GetSpriteFrameIndex(SpriteFrame* spriteFrame) const
{
	if (spriteFrame == NULL)
	{
		SEthrow(ArgumentNullException());
		return -1;
	}
	else
	{
		return _SpriteFrames.IndexOf(spriteFrame);
	}
}

SpriteFrame* Sprite::GetSpriteFrame(int index) const
{
	return _SpriteFrames[index];
}

bool Sprite::SetSpriteFrame(int index, SpriteFrame* spriteFrame)
{
	if (spriteFrame == NULL)
	{
		SEthrow(ArgumentNullException());
		return false;
	}
	else
	{
		_SpriteFrames.SetItem(index, spriteFrame);
		return true;
	}
}

void Sprite::SetGlobalAnimationDelay(real64 animationDelay)
{
	SpriteFrameList::Iterator it = _SpriteFrames.GetIterator();
	while (it.Next())
	{
		it.Current()->SetAnimationDelay(animationDelay);
	}
}


SpriteFrameBuilder::SpriteFrameBuilder() :
	FrameOffsetX(0),
	FrameOffsetY(0),
	FrameWidth(0),
	FrameHeight(0),
	FrameSpanX(0),
	FrameSpanY(0),
	FrameColumns(0),
	FrameRows(0),
	UseSize(false),
	UseFullImage(true)
{
}

int32 SpriteFrameBuilder::Build(Sprite* sprite, Image* image)
{
	if (sprite == NULL || image == NULL)
		return 0;

	// The size of each frame is needed when not using the whole image
	if (!UseSize && !UseFullImage)
		return 0;

	if (FrameOffsetX < 0)
		FrameOffsetX = 0;
	if (FrameOffsetY < 0)
		FrameOffsetY = 0;

	int32 count = 0;
	int32 offsetx, offsety;
	int32 width, height;
	int32 columns, rows;

	if (UseFullImage)
	{
		offsetx = offsety = 0;
		if (UseSize)
		{
			width = FrameWidth;
			height = FrameHeight;
			columns = image->GetWidth() / FrameWidth;
			rows = image->GetHeight() / FrameHeight;
		}
		else
		{
			width = image->GetWidth() / FrameColumns;
			height = image->GetHeight() / FrameRows;
			columns = FrameColumns;
			rows = FrameRows;
		}
	}
	else
	{
		offsetx = FrameOffsetX;
		offsety = FrameOffsetY;
		width = FrameWidth;
		height = FrameHeight;
		columns = FrameColumns;
		rows = FrameRows;
	}

	if (width <= 0 || height <= 0 || columns <= 0 || rows <= 0)
		return 0;

	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < columns; i++)
		{
			SpriteFrame* frame = new SpriteFrame();
			frame->SetImage(image);
			frame->SetSourceRect(RectangleInt(offsetx + width*i, offsety + height*j, width, height));
			sprite->AddSpriteFrame(frame);
			count++;

			offsetx += FrameSpanX;
		}

		offsety += FrameSpanY;
	}

	return count;
}

}

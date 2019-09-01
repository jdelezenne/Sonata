/*=============================================================================
Sprite.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SPRITE_H_
#define _SE_SPRITE_H_

#include "Graphics/Common.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/System/Texture.h"
#include "Graphics/States/AlphaState.h"

namespace SonataEngine
{

/** Sprite Animation Mode. */
enum SpriteAnimationMode
{
	SpriteAnimationMode_Stop,
	SpriteAnimationMode_Loop,
	SpriteAnimationMode_Reverse
};

/**
	@brief Sprite frame.

	Class for a frame of an animated sprite.
*/
class SE_GRAPHICS_EXPORT SpriteFrame : public RefObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	SpriteFrame();
	virtual ~SpriteFrame();
	//@}

	const RectangleInt& GetSourceRect() const { return _SourceRect; }
	void SetSourceRect(const RectangleInt& sourceRect) { _SourceRect = sourceRect; }

	Texture* GetTexture() const { return _texture; }
	void SetTexture(Texture* texture) { _texture = texture; }
	void SetImage(Image* image);

	real64 GetAnimationDelay() const { return _AnimationDelay; }
	void SetAnimationDelay(real64 animationDelay) { _AnimationDelay = animationDelay; }

protected:
	RectangleInt _SourceRect;
	TexturePtr _texture;
	real64 _AnimationDelay;
};

/**
	@brief Sprite.
	2D sprites.
*/
class SE_GRAPHICS_EXPORT Sprite : public RefObject
{
public:
	typedef Array<SpriteFrame*> SpriteFrameList;

	/** @name Constructors / Destructor. */
	//@{
	Sprite();
	virtual ~Sprite();
	//@}

	virtual void Update(const TimeValue& timeValue);

	virtual void Render();

	/** @name Frames. */
	//@{
	int GetSpriteFrameCount() const;
	bool AddSpriteFrame(SpriteFrame* spriteFrame);
	bool InsertSpriteFrame(int index, SpriteFrame* spriteFrame);
	bool ReplaceSpriteFrame(SpriteFrame* previous, SpriteFrame* spriteFrame);
	bool RemoveSpriteFrame(SpriteFrame* spriteFrame);
	void RemoveAllSpriteFrames();
	int GetSpriteFrameIndex(SpriteFrame* spriteFrame) const;
	SpriteFrame* GetSpriteFrame(int index) const;
	bool SetSpriteFrame(int index, SpriteFrame* spriteFrame);
	//@}

	/** @name Properties. */
	//@{
	const Vector2& GetPosition() const { return _Position; }
	void SetPosition(const Vector2& value) { _Position = value; }

	const Vector2& GetOffset() const { return _Offset; }
	void SetOffset(const Vector2& value) { _Offset = value; }

	real GetDepth() const { return _Depth; }
	void SetDepth(real value) { if (_Depth != value) { _Depth = value; _Invalidate(); } }

	const Vector2& GetSize() const { return _Size; }
	void SetSize(const Vector2& value) { if (_Size != value) { _Size = value; _Invalidate(); } }

	const Vector2& GetScale() const { return _Scale; }
	void SetScale(const Vector2& value) { if (_Scale != value) { _Scale = value; _Invalidate(); } }

	Vector2 GetImageScale() const { return _ImageScale; }
	void SetImageScale(const Vector2& value) { _ImageScale = value; }

	const Color32& GetColor() const { return _Color; }
	void SetColor(const Color32& value) { if (_Color != value) { _Color = value; _Invalidate(); } }

	const AlphaState& GetAlphaState() const { return AlphaState; }
	void SetAlphaState(const AlphaState& value) { AlphaState = value; }

	int32 GetCurrentFrame() const { return _currentFrame; }
	void SetCurrentFrame(int32 value)
	{
		if (value < 0 || value >= _SpriteFrames.Count())
			return;

		_currentFrame = value;
		if (_CachedSourceRect != _SpriteFrames[_currentFrame]->GetSourceRect())
		{
			_UpdateMesh();
			_CachedSourceRect = _SpriteFrames[_currentFrame]->GetSourceRect();
		}

	}

	bool GetBackwardAnimation() const { return _BackwardAnimation; }
	void SetBackwardAnimation(bool value) { _BackwardAnimation = value; }

	SpriteAnimationMode GetAnimationMode() const { return _AnimationMode; }
	void SetAnimationMode(SpriteAnimationMode value) { _AnimationMode = value; }

	void SetGlobalAnimationDelay(real64 value);
	//@}

public:
	AlphaState AlphaState;

private:
	void _Invalidate();
	void _UpdateMesh();

	MeshPtr _mesh;
	RectangleInt _CachedSourceRect;

protected:
	bool _Invalid;
	Vector2 _Position;
	Vector2 _Offset;
	real _Depth;
	Vector2 _Rotation;
	Vector2 _Size;
	Vector2 _Scale;
	Vector2 _ImageScale;
	Color32 _Color;
	SpriteFrameList _SpriteFrames;
	int32 _currentFrame;
	bool _BackwardAnimation;
	SpriteAnimationMode _AnimationMode;
};

class SE_GRAPHICS_EXPORT SpriteFrameBuilder
{
public:
	/** @name Constructors / Destructor. */
	//@{
	SpriteFrameBuilder();
	//@}

	int32 Build(Sprite* sprite, Image* image);

	int32 FrameOffsetX;
	int32 FrameOffsetY;
	int32 FrameWidth;
	int32 FrameHeight;
	int32 FrameSpanX;
	int32 FrameSpanY;
	int32 FrameColumns;
	int32 FrameRows;
	bool UseSize;
	bool UseFullImage;
};

typedef SmartPtr<Sprite> SpritePtr;

}

#endif 

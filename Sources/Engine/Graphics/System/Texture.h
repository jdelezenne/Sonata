/*=============================================================================
Texture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TEXTURE_H_
#define _SE_TEXTURE_H_

#include "Graphics/Common.h"
#include "Graphics/Image.h"
#include "Graphics/System/HardwareBuffer.h"

namespace SonataEngine
{

/** Types of texture. */
enum TextureType
{
	/// 1D texture.
	TextureType_Texture1D,

	/// 2D texture.
	TextureType_Texture2D,

	/// Cube texture.
	TextureType_TextureCube,

	/// Volume texture.
	TextureType_TextureVolume
};

/** Texture usages. */
enum TextureUsage
{
	/// No usage type. 
	TextureUsage_Static,

	/// Dynamic memory use.
	TextureUsage_Dynamic,

	/// Specifies that the texture will be a render target.
	TextureUsage_RenderTarget,

	/// Specifies that the texture will be a depth or stencil buffer.
	TextureUsage_DepthStencil
};

/** The different faces of a cube texture. */
enum CubeTextureFace
{
	/// Positive X face.
	CubeTextureFace_PositiveX,

	/// Negative X face.
	CubeTextureFace_NegativeX,

	/// Positive Y face.
	CubeTextureFace_PositiveY,

	/// Negative Y face.
	CubeTextureFace_NegativeY,

	/// Positive Z face.
	CubeTextureFace_PositiveZ,

	/// Negative Z face.
	CubeTextureFace_NegativeZ
};

/** Texture. */
class SE_GRAPHICS_EXPORT Texture : public RefObject
{
protected:
	TextureType _textureType;
	TextureUsage _textureUsage;
	PixelFormat _format;
	int32 _width;
	int32 _height;
	int32 _depth;
	int32 _bitsPerPixel;
	int32 _mipLevels;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Texture();

	/** Destructor. */
	virtual ~Texture();
	//@}

	/** Clones the texture. */
	virtual Texture* Clone() const = 0;

	/** @name Properties. */
	//@{
	/** Gets the type of the texture. */
	TextureType GetTextureType() const { return _textureType; }

	/** Gets the usage of the texture. */
	TextureUsage GetTextureUsage() const { return _textureUsage; }

	/** Gets the pixel format of the texture. */
	const PixelFormat& GetFormat() const { return _format; }

	/** Gets the width of the texture. */
	int32 GetWidth() const { return _width; }

	/** Gets the height of the texture. */
	int32 GetHeight() const { return _height; }

	/** Gets the depth of the texture.
		@remark For volume textures.
	*/
	int32 GetDepth() const { return _depth; }

	/** Gets the number of bits per pixel in the texture. */
	int32 GetBitsPerPixel() const { return _bitsPerPixel; }

	/** Gets the number of mip levels in the texture. */
	int32 GetMipLevels() const { return _mipLevels; }

	/** Sets the number of mip levels in the texture. */
	void SetMipLevels(int32 value) { _mipLevels = value; }

	/** Gets the size of the texture. */
	int32 GetSize() const { return (_width * _height * _depth); }

	/** Gets the number of face in the texture. */
	int32 GetFaceCount() const;
	//@}

	virtual bool Create(TextureType textureType, PixelFormat format, int width, int height, int depth, int mipLevels, TextureUsage usage) = 0;
	virtual bool Create(Image* image, TextureUsage usage) = 0;
	virtual bool Destroy() = 0;

	/**
		Returns whether the texture is mapped.
		@return true if successful; otherwise, false.
	*/
	virtual bool IsMapped() = 0;

	/**
		Get a pointer to buffer memory to modify its contents.
		@param mode Permissions.
		@param data [out] Pointer to the buffer data.
		@return true if successful; otherwise, false.
	*/
	virtual bool Map(HardwareBufferMode mode, void** data, int mipLevel = 0) = 0;
	virtual bool MapCubeFace(HardwareBufferMode mode, CubeTextureFace face, void** data, int mipLevel = 0) = 0;

	/** Unmap the buffer. */
	virtual void Unmap() = 0;
};

typedef SmartPtr<Texture> TexturePtr;

}

#endif

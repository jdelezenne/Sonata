/*=============================================================================
SamplerState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SAMPLERSTATE_H_
#define _SE_SAMPLERSTATE_H_

#include "Graphics/Common.h"
#include "Graphics/System/Texture.h"

namespace SonataEngine
{

/** Sampler state. */
class SE_GRAPHICS_EXPORT SamplerState
{
private:
	TexturePtr _texture;

public:
 	/** @name Properties. */
	//@{
	/// Name.
	String Name;

	/// Minification filter.
	TextureFilterType MinFilter;

	/// Magification filter.
	TextureFilterType MagFilter;

	/// Mipmap filter.
	TextureFilterType MipFilter;

	/// Filter will output a comparison value between 0 and 1.
	bool Comparison;

	/// For use in pixel shaders with textures that have the R1_UNORM format.
	bool FilterTexture1Bit;

	/// Addressing mode for the u texture coordinate.
	TextureAddressMode AddressModeU;

	/// Addressing mode for the v texture coordinate.
	TextureAddressMode AddressModeV;

	/// Addressing mode for the w texture coordinate.
	TextureAddressMode AddressModeW;

	/// Offset from the calculated mip map level.
	real32 MipLODBias;

	/// Clamping value used if TextureFilterType_Anisotropic is specified in the sampler filter.
	uint32 MaxAnisotropy;

	/// Comparision function to use if the Comparison is enabled.
	ComparisonFunction ComparisonFunction;

	/// Color to use if TextureAddressMode_Border is specified for the addressing modes.
	Color32 BorderColor;

	/// Lower end of the desired mip map range, where 0 is the largest and most detailed mip map level and any level higher than that is less detailed.
	real32 MinLOD;

	/// Upper end of the desired mip map range, where 0 is the largest and most detailed mip map level and any level higher than that is less detailed.
	real32 MaxLOD;
	//@}

public:
	/** @name Constructor. */
	//@{
	SamplerState();
	//@}

	/// Texture.
	Texture* GetTexture() const { return _texture; }
	void SetTexture(Texture* value) { _texture = value; }
};

SE_DECLARE_STRUCT(SamplerState);
SE_BEGIN_STRUCT(SamplerState);
	SE_StructField(Name, String);
	//SE_StructField(_texture, Texture);
	SE_StructField(MinFilter, TextureFilterType);
	SE_StructField(MagFilter, TextureFilterType);
	SE_StructField(MipFilter, TextureFilterType);
	SE_StructField(Comparison, bool);
	SE_StructField(FilterTexture1Bit, bool);
	SE_StructField(AddressModeU, TextureAddressMode);
	SE_StructField(AddressModeV, TextureAddressMode);
	SE_StructField(AddressModeW, TextureAddressMode);
	SE_StructField(MaxAnisotropy, real32);
	SE_StructField(ComparisonFunction, ComparisonFunction);
	SE_StructField(BorderColor, Color32);
	SE_StructField(MinLOD, real32);
	SE_StructField(MaxLOD, real32);
SE_END_STRUCT(SamplerState);

}

#endif

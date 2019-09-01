/*=============================================================================
SamplerState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SamplerState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(SamplerState);

SamplerState::SamplerState() :
	_texture(NULL),
	MinFilter(TextureFilterType_Linear),
	MagFilter(TextureFilterType_Linear),
	MipFilter(TextureFilterType_None),
	Comparison(false),
	FilterTexture1Bit(false),
	AddressModeU(TextureAddressMode_Wrap),
	AddressModeV(TextureAddressMode_Wrap),
	AddressModeW(TextureAddressMode_Wrap),
	MipLODBias(0.0f),
	MaxAnisotropy(1),
	ComparisonFunction(ComparisonFunction_Always),
	BorderColor(Color32::Black),
	MinLOD(0.0f),
	MaxLOD(0.0f)
{
}

}

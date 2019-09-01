/*=============================================================================
TextureState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TextureState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(TextureState);

TextureState::TextureState() :
	TextureCoordinateIndex(0),
	TextureCoordinateGeneration(TextureCoordinateMode_PassThru),
	TextureTransform(TextureTransform_Disable),
	Offset(Vector2::Zero),
	Tile(Vector2::One),
	Angle(Vector2::Zero),
	ColorOperation(TextureOperation_Modulate),
	ColorArgument1(TextureArgument_TextureColor),
	ColorArgument2(TextureArgument_Diffuse),
	AlphaOperation(TextureOperation_SelectArg1),
	AlphaArgument1(TextureArgument_TextureColor),
	AlphaArgument2(TextureArgument_Diffuse),
	BumpEnvironmentMaterial00(0.0f),
	BumpEnvironmentMaterial01(0.0f),
	BumpEnvironmentMaterial10(0.0f),
	BumpEnvironmentMaterial11(0.0f),
	BumpEnvironmentLuminanceScale(0.0f),
	BumpEnvironmentLuminanceOffset(0.0f),
	ColorArgument0(TextureArgument_Current),
	AlphaArgument0(TextureArgument_Current),
	ResultArgument(TextureArgument_Current),
	ConstantColor(Color32::Black)
{
}

}

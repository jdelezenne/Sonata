/*=============================================================================
TextureState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TEXTURESTATE_H_
#define _SE_TEXTURESTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	Texture state.
	@todo Modifier flags: AlphaReplicate Complement
*/
class SE_GRAPHICS_EXPORT TextureState
{
public:
 	/** @name Properties. */
	//@{
	/// Name.
	String Name;

	/// Defines the index of the texture coordinate set to use with the current texture stage.
	int32 TextureCoordinateIndex;

	/// Defines the automatic generation of the input texture coordinates for a texture transformation.
	TextureCoordinateMode TextureCoordinateGeneration;

	/// Controls the transformation of texture coordinates for the current texture stage. 
	TextureTransform TextureTransform;

	/// Offset.
	Vector2 Offset;

	/// Tile.
	Vector2 Tile;

	/// Angle.
	Vector2 Angle;

	/// Defines the texture-stage state as a texture color-blending operation identified by the TextureOperation enumeration.
	TextureOperation ColorOperation;

	/// Defines the texture-stage state as the first color argument for the stage identified by the TextureArgument enumeration.
	TextureArgument ColorArgument1;

	/// Defines the texture-stage state as the second color argument for the stage identified by the TextureArgument enumeration.
	TextureArgument ColorArgument2;

	/// Defines the texture-stage state as a texture alpha-blending operation identified by the TextureOperation enumeration.
	TextureOperation AlphaOperation;

	/// Defines the texture-stage state as the first alpha argument for the stage identified by the TextureArgument enumeration.
	TextureArgument AlphaArgument1;

	/// Defines the texture-stage state as the second alpha argument for the stage identified by the TextureArgument enumeration.
	TextureArgument AlphaArgument2;

	/// Defines the texture-stage state as a floating-point value for the [0][0] coefficient in a bump-mapping matrix.
	real32 BumpEnvironmentMaterial00;
	/// Defines the texture-stage state as a floating-point value for the [0][1] coefficient in a bump-mapping matrix.
	real32 BumpEnvironmentMaterial01;

	/// Defines the texture-stage state as a floating-point value for the [1][0] coefficient in a bump-mapping matrix.
	real32 BumpEnvironmentMaterial10;

	/// Defines the texture-stage state as a floating-point value for the [1][1] coefficient in a bump-mapping matrix.
	real32 BumpEnvironmentMaterial11;

	/// Defines a floating-point scale value for bump-map luminance.
	real32 BumpEnvironmentLuminanceScale;

	/// Defines a floating-point offset value for bump-map luminance.
	real32 BumpEnvironmentLuminanceOffset;

	/// Defines settings for the third color operand for triadic (multiply, add, and linear interpolation) operations identified by the TextureArgument enumeration.
	TextureArgument ColorArgument0;

	/// Defines settings for the alpha channel selector operand for triadic (multiply, add, and linear 	/// interpolation) operations identified by the TextureArgument enumeration.
	TextureArgument AlphaArgument0;

	/// Selects the destination register for the result of the current stage identified by TextureArgument.
	TextureArgument ResultArgument;

	/// Defines the per-stage constant color.
	Color32 ConstantColor;
	//@}

public:
	/** @name Constructor. */
	//@{
	TextureState();
	//@}
};

SE_DECLARE_STRUCT(TextureState);
SE_BEGIN_STRUCT(TextureState);
	SE_StructField(Name, String);
	SE_StructField(TextureCoordinateIndex, int32);
	SE_StructField(TextureCoordinateGeneration, TextureCoordinateMode);
	SE_StructField(TextureTransform, TextureTransform);
	SE_StructField(Offset, Vector2);
	SE_StructField(Tile, Vector2);
	SE_StructField(Angle, Vector2);
	SE_StructField(ColorOperation, TextureOperation);
	SE_StructField(ColorArgument1, TextureArgument);
	SE_StructField(ColorArgument2, TextureArgument);
	SE_StructField(AlphaOperation, TextureOperation);
	SE_StructField(AlphaArgument1, TextureArgument);
	SE_StructField(AlphaArgument2, TextureArgument);
	SE_StructField(BumpEnvironmentMaterial00, real32);
	SE_StructField(BumpEnvironmentMaterial01, real32);
	SE_StructField(BumpEnvironmentMaterial10, real32);
	SE_StructField(BumpEnvironmentMaterial11, real32);
	SE_StructField(BumpEnvironmentLuminanceScale, real32);
	SE_StructField(BumpEnvironmentLuminanceOffset, real32);
	SE_StructField(ColorArgument0, TextureArgument);
	SE_StructField(AlphaArgument0, TextureArgument);
	SE_StructField(ResultArgument, TextureArgument);
	SE_StructField(ConstantColor, Color32);
SE_END_STRUCT(TextureState);

}

#endif

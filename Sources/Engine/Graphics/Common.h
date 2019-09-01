/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GRAPHICS_COMMON_H_
#define _SE_GRAPHICS_COMMON_H_

#include "Core/Core.h"

#define SE_ID_RENDERSYSTEM SonataEngine::SE_ID(0x68516dd5,0x21cf4c49)
#define SE_ID_SHADERSYSTEM SonataEngine::SE_ID(0x3562ec97,0x76f9421a)

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#	ifdef SE_STATIC
#		define SE_GRAPHICS_EXPORT
#	else
#		ifdef SE_ENGINEGRAPHICS
#			define SE_GRAPHICS_EXPORT __declspec(dllexport)
#		else
#			define SE_GRAPHICS_EXPORT __declspec(dllimport)
#		endif 
#	endif 
#else
#	define SE_GRAPHICS_EXPORT
#endif

namespace SonataEngine
{

/** Texture filter type. */
enum TextureFilterType
{
	/// Mipmapping disabled.
	TextureFilterType_None,

	/// Point filtering used as a texture magnification or minification filter.
	TextureFilterType_Point,

	/// Bilinear interpolation filtering used as a texture magnification or minification filter.
	TextureFilterType_Linear,

	/// Anisotropic texture filtering used as a texture magnification or minification filter.
	TextureFilterType_Anisotropic
};

/** Texture address mode. */
enum TextureAddressMode
{
	/// Tiles the texture at every integer junction.
	TextureAddressMode_Wrap,

	/// Similar to Wrap, except that the texture is flipped at every integer junction.
	TextureAddressMode_Mirror,

	/** Texture coordinates outside the range [0.0, 1.0] are set to
        the texture color at 0.0 or 1.0, respectively. */
	TextureAddressMode_Clamp,

	/// Texture coordinates outside the range [0.0, 1.0] are set to the border color.
	TextureAddressMode_Border,

	/// Similar to Mirror and Clamp (D3D specific).
	TextureAddressMode_MirrorOnce
};

enum IndexBufferFormat
{
	IndexBufferFormat_Int16,
	IndexBufferFormat_Int32
};

/** Types of primitive topology. */
enum PrimitiveType
{
	/// The default topology for a primitive.
    PrimitiveType_Undefined,

	/// Primitive defined by point lists.
    PrimitiveType_PointList,

	/// Primitive defined by line lists.
    PrimitiveType_LineList,

	/// Primitive defined by line strips.
    PrimitiveType_LineStrip,

	/// Primitive defined by triangle lists.
    PrimitiveType_TriangleList,

	/// Primitive defined by triangle strips.
    PrimitiveType_TriangleStrip,

	/// Primitive defined by triangle fans.
    PrimitiveType_TriangleFan,

	/// Primitive defined by quad lists.
	PrimitiveType_QuadList,

	/// Primitive defined by quad strips.
    PrimitiveType_QuadStrip,

	/// Primitive defined by rectangle lists.
    PrimitiveType_RectList,

	/// Primitive defined by polygon lists.
    PrimitiveType_Polygon
};

enum FillMode
{
	/// Fill points.
	FillMode_Point,

	/// Fill wireframes.
	FillMode_WireFrame,

	/// Fill solids.
	FillMode_Solid
};

enum ShadeMode
{
	/// Flat shading mode.
	ShadeMode_Flat,

	/// Smooth shading mode.
	ShadeMode_Smooth
};

enum CullMode
{
	/// Do not cull back faces.
	CullMode_None,

	/// Cull triangles facing the camera.
	CullMode_Front,

	/// Cull triangles facing away from the camera.
	CullMode_Back
};

enum ColorFlag
{
	/// Red channel of a buffer.
	ColorFlag_Red = 1,

	/// Green channel of a buffer.
	ColorFlag_Green = 2,

	/// Blue channel of a buffer.
	ColorFlag_Blue = 4,

	/// The red, green, and blue channels of a buffer.
	ColorFlag_RedGreenBlue = 7,

	/// Alpha channel of a buffer.
	ColorFlag_Alpha = 8,

	/// All channels of a buffer.
	ColorFlag_All = 15
};

enum ComparisonFunction
{
	ComparisonFunction_Never,
	ComparisonFunction_Less,
	ComparisonFunction_Equal,
	ComparisonFunction_LessEqual,
	ComparisonFunction_Greater,
	ComparisonFunction_NotEqual,
	ComparisonFunction_GreaterEqual,
	ComparisonFunction_Always
};

enum StencilOperation
{
	/// Does not update the stencil-buffer entry. This is the default value.
	StencilOperation_Keep,
	
	/// Sets the stencil-buffer entry to 0.
	StencilOperation_Zero,
	
	/// Replaces the stencil-buffer entry with a reference value.
	StencilOperation_Replace,
	
	/// Increments the stencil-buffer entry, clamping to the maximum value.
	StencilOperation_IncrementSaturation,

	/// Decrements the stencil-buffer entry, clamping to 0.
	StencilOperation_DecrementSaturation,
	
	/// Inverts the bits in the stencil-buffer entry.
	StencilOperation_Invert,
	
	/// Increments the stencil-buffer entry, wrapping to 0 if the new value exceeds the maximum value.
	StencilOperation_Increment,

	/// Decrements the stencil-buffer entry, wrapping to the maximum value if the new value is less than 0.
	StencilOperation_Decrement
};

enum BlendMode
{
	/// Blend factor is (0, 0, 0, 0).
	BlendMode_Zero,
	
	/// Blend factor is (1, 1, 1, 1).
	BlendMode_One,
	
	/// Blend factor is (Rs, Gs, Bs, As).
	BlendMode_SourceColor,
	
	/// Blend factor is (1 - Rs, 1 - Gs, 1 - Bs, 1 - As).
	BlendMode_InvSourceColor,
	
	/// Blend factor is (As, As, As, As).
	BlendMode_SourceAlpha,
	
	/// Blend factor is ( 1 - As, 1 - As, 1 - As, 1 - As).
	BlendMode_InvSourceAlpha,
	
	/// Blend factor is (Ad, Ad, Ad, Ad).
	BlendMode_DestinationAlpha,
	
	/// Blend factor is (1 - Ad, 1 - Ad, 1 - Ad, 1 - Ad).
	BlendMode_InvDestinationAlpha,
	
	/// Blend factor is (Rd, Gd, Bd, Ad).
	BlendMode_DestinationColor,
	
	/// Blend factor is (1 - Rd, 1 - Gd, 1 - Bd, 1 - Ad).
	BlendMode_InvDestinationColor,
	
	/// Blend factor is (f, f, f, 1); f = min(A, 1 - Ad).
	BlendMode_SourceAlphaSat,
	
	/// Constant color blending factor used by the frame-buffer blender.
	BlendMode_BlendFactor,
	
	/// Inverted constant color blending factor used by the frame-buffer blender.
	BlendMode_InvBlendFactor
};

enum BlendOperation
{
	/// Result is the destination added to the source. Result = Source + Destination
	BlendOperation_Add,
	
	/// Result is the destination subtracted from the source. Result = Source - Destination
	BlendOperation_Subtract,

	/// Result is the source subtracted from the destination. Result = Destination - Source
	BlendOperation_RevSubtract,

	/// Result is the minimum of the source and destination. Result = MIN(Source, Destination)
	BlendOperation_Min,

	/// Result is the maximum of the source and destination. Result = MAX(Source, Destination)
	BlendOperation_Max
};

/** Fog mode. */
enum FogMode
{
	/// No fog effect.
	FogMode_None,

	/// Fog effect intensifies in a linear manner between the start and end points.
	FogMode_Linear,

	/// Fog effect intensifies exponentially.
	FogMode_Exponential,

	/// Fog effect intensifies exponentially with the square of the distance.
	FogMode_Exponential2
};

/** Defines automatic generation of the input texture coordinates. */
enum TextureCoordinateMode
{
	/// Uses the specified texture coordinates contained in the vertex format.
	TextureCoordinateMode_PassThru,

	/// Uses the vertex normal, transformed to camera space, as the input texture coordinates for the current stage's texture transformation.
	TextureCoordinateMode_CameraSpaceNormal,

	/// Uses the vertex position, transformed to camera space, as the input texture coordinates for the current stage's texture transformation.
	TextureCoordinateMode_CameraSpacePosition,

	/// Uses the reflection vector, transformed to camera space, as the input texture coordinate for the current stage's texture transformation. The reflection vector is computed from the input vertex position and normal vector.
	TextureCoordinateMode_CameraSpaceReflectionVector,

	/// Uses the specified texture coordinates for sphere mapping.
	TextureCoordinateMode_SphereMap
};

/** Defines texture coordinate transformation values. */
enum TextureTransform
{
	/// Texture coordinates are passed directly to the rasterizer.
	TextureTransform_Disable,

	/// 1D texture coordinates are passed to the rasterizer.
	TextureTransform_Count1,

	/// 2D texture coordinates are passed to the rasterizer.
	TextureTransform_Count2,

	/// 3D texture coordinates are passed to the rasterizer.
	TextureTransform_Count3,

	/// 4D texture coordinates are passed to the rasterizer.
	TextureTransform_Count4,

	/// All texture coordinates are divided by the last element before being passed to the rasterizer.
	TextureTransform_Projected
};

/** Defines per-stage texture-blending operations. */
enum TextureOperation
{
	/** Disables output from the texture stage and all stages with a higher index. */
	TextureOperation_Disable,

	/** Uses this texture stage's first color or alpha argument, unmodified, as the output.
		SRGBA = Arg1
	*/
	TextureOperation_SelectArg1,

	/** Uses this texture stage's second color or alpha argument, unmodified, as the output.
		SRGBA = Arg2
	*/
	TextureOperation_SelectArg2,

	/** Multiplies the components of the arguments.
		SRGBA = Arg1 * Arg2
	*/
	TextureOperation_Modulate,

	/** Multiplies the components of the arguments, then shifts the products 1 bit to the left (effectively multiplying them by 2) for brightening.
		SRGBA = (Arg1 * Arg2) << 1
	*/
	TextureOperation_Modulate2X,

	/** Multiplies the components of the arguments, then shifts the products 2 bits to the left (effectively multiplying them by 4) for brightening.
		SRGBA = (Arg1 * Arg2) << 2
	*/
	TextureOperation_Modulate4X,

	/** Adds the components of the arguments.
		SRGBA = Arg1 + Arg2
	*/
	TextureOperation_Add,

	/** Adds the components of the arguments with a -0.5 bias, making the effective range of values -0.5 through 0.5.
		SRGBA = Arg1 + Arg2 -0.5
	*/
	TextureOperation_AddSigned,

	/** Adds the components of the arguments with a -0.5 bias and shifts the products 1 bit to the left.
		SRGBA = (Arg1 + Arg2 - 0.5) << 1
	*/
	TextureOperation_AddSigned2X,

	/** Subtracts the components of the second argument from those of the first argument.
		SRGBA = Arg1 - Arg2
	*/
	TextureOperation_Subtract,

	/** Adds the first and second arguments, then subtracts their product from the sum.
		SRGBA = Arg1 + Arg2 - Arg1 * Arg2
	*/
	TextureOperation_AddSmooth,

	/** Performs linear blending on the current texture stage, using the interpolated alpha from each vertex.
		SRGBA = Arg1 * (Alpha) + Arg2 * (1 - Alpha)
	*/
	TextureOperation_BlendDiffuseAlpha,

	/** Performs linear blending on the current texture stage, using the interpolated alpha from each vertex.
		SRGBA = Arg1 * (Alpha) + Arg2 * (1 - Alpha)
	*/
	TextureOperation_BlendTextureAlpha,

	/** Performs linear blending on the current texture stage, using a scalar alpha set with the TextureFactor property.
		SRGBA = Arg1 * (Alpha) + Arg2 * (1 - Alpha)
	*/
	TextureOperation_BlendFactorAlpha,

	/** Performs linear blending on the current texture stage that uses a premultiplied alpha.
		SRGBA = Arg1 + Arg2 * (1 - Alpha)
	*/
	TextureOperation_BlendTextureAlphaPM,

	/** Performs linear blending on the current texture stage, using the alpha taken from the previous texture stage.
		SRGBA = Arg1 * (Alpha) + Arg2 * (1 - Alpha)
	*/
	TextureOperation_BlendCurrentAlpha,

	/** This flag is set in stage n, the output of which is arg1. If stage n + 1 contains a texture, any Current value in stage n + 1 is premultiplied by that texture.
	*/
	TextureOperation_PreModulate,

	/** Modulates the color of the second argument using the alpha of the first argument, then adds the result to argument one. This operation is supported only for color operations.
		SRGBA = Arg1RGB + Arg1A * Arg2RGB
	*/
	TextureOperation_ModulateAlphaAddColor,

	/** Similar to ModulateAlphaAddColor, but uses the inverse of the alpha of the first argument. This operation is supported only for color operations.
		SRGBA = Arg1RGB * Arg2RGB + Arg1A
	*/
	TextureOperation_ModulateColorAddAlpha,

	/** Similar to ModulateColorAddAlpha, but uses the inverse of the color of the first argument. This operation is supported only for color operations.
		SRGBA = (1 - Arg1A) * Arg2RGB + Arg1RGB
	*/
	TextureOperation_ModulateInvAlphaAddColor,

	/** Similar to ModulateColorAddAlpha, but uses the inverse of the color of the first argument. This operation is supported only for color operations.
		SRGBA = (1 - Arg1RGB) * Arg2RGB + Arg1A
	*/
	TextureOperation_ModulateInvColorAddAlpha,

	/** Performs bump mapping on each pixel, using the environment map in the next texture stage without luminance. This operation is supported only for color operations.
	*/
	TextureOperation_BumpEnvironmentMap,

	/** PPerforms per-pixel bump mapping, using the environment map in the next texture stage with luminance. This operation is supported only for color operations.
	*/
	TextureOperation_BumpEnvironmentMapLuminance,

	/** Modulates the components of each argument as signed components, adds their products, and replicates the sum to all color channels, including alpha. This operation is supported for color and alpha operations.
		SRGBA = (Arg1R * Arg2R + Arg1G * Arg2G + Arg1B * Arg2B)
	*/
	TextureOperation_DotProduct3,

	/** Performs a multiply-accumulate operation, in which it multiplies the last two arguments, adds them to the remaining input/source argument, and places that into the result.
		SRGBA = Arg1 + Arg2 *Arg3
	*/
	TextureOperation_MultiplyAdd,

	/** Performs linear interpolation between the second and third source arguments by a proportion specified in the first source argument.
		SRGBA = (Arg1) * Arg2 + (1- Arg1) * Arg3
	*/
	TextureOperation_Lerp
};

/** Defines the supported texture arguments. */
enum TextureArgument
{
	/// Specifies the diffuse color interpolated from vertex components during Smooth shading. If the vertex does not contain a diffuse color, the default color is 0xFFFFFFFF. Permissions are read-only.
	TextureArgument_Diffuse,
	/// Gets the current element in the collection.
	TextureArgument_Current,
	/// Contains the texture color for the current texture stage. Permissions are read-only.
	TextureArgument_TextureColor,
	/// Specifies the texture factor set in the RenderStateManager.TextureFactor property. Permissions are read-only.
	TextureArgument_TFactor,
	/// Specifies the specular color interpolated from vertex components during Smooth shading. If the vertex does not contain a specular color, the default color is 0xFFFFFFFF. Permissions are read-only.
	TextureArgument_Specular,
	/// Specifies a temporary register color for read or write. Temp is supported if the SupportsTextureStageStateArgumentTemp device capability is present. The default value for the register is (0.0, 0.0, 0.0, 0.0). Permissions are read/write.
	TextureArgument_Temp,
	/// Select a constant from a texture stage.
	TextureArgument_Constant,
	/// Mask value for all arguments; not used when setting texture arguments.
	TextureArgument_SelectMask,
	/// Take the complement, or 1.0 - x. This is a read modifier.
	TextureArgument_Complement,
	/// Replicate the alpha value to the color components. During sampling, AlphaReplicate acts as a read modifier.
	TextureArgument_AlphaReplicate
};

SE_BEGIN_ENUM(TextureFilterType);
	SE_Enum(None);
	SE_Enum(Point);
	SE_Enum(Linear);
	SE_Enum(Anisotropic);
SE_END_ENUM(TextureFilterType);

SE_BEGIN_ENUM(TextureAddressMode);
	SE_Enum(Wrap);
	SE_Enum(Mirror);
	SE_Enum(Clamp);
	SE_Enum(Border);
	SE_Enum(MirrorOnce);
SE_END_ENUM(TextureAddressMode);

SE_BEGIN_ENUM(IndexBufferFormat);
	SE_Enum(Int16);
	SE_Enum(Int32);
SE_END_ENUM(IndexBufferFormat);

SE_BEGIN_ENUM(PrimitiveType);
	SE_Enum(PointList);
	SE_Enum(LineList);
	SE_Enum(LineStrip);
	SE_Enum(TriangleList);
SE_END_ENUM(PrimitiveType);

SE_BEGIN_ENUM(FillMode);
	SE_Enum(Point);
	SE_Enum(WireFrame);
	SE_Enum(Solid);
SE_END_ENUM(FillMode);

SE_BEGIN_ENUM(ShadeMode);
	SE_Enum(Flat);
	SE_Enum(Smooth);
SE_END_ENUM(ShadeMode);

SE_BEGIN_ENUM(CullMode);
	SE_Enum(None);
	SE_Enum(Front);
	SE_Enum(Back);
SE_END_ENUM(CullMode);

SE_BEGIN_ENUM(ColorFlag);
	SE_EnumValue(Red, 1);
	SE_EnumValue(Green, 2);
	SE_EnumValue(Blue, 4);
	SE_EnumValue(RedGreenBlue, 7);
	SE_EnumValue(Alpha, 8);
	SE_EnumValue(All, 15);
SE_END_ENUM(ColorFlag);

SE_BEGIN_ENUM(ComparisonFunction);
	SE_Enum(Never);
	SE_Enum(Less);
	SE_Enum(Equal);
	SE_Enum(LessEqual);
	SE_Enum(Greater);
	SE_Enum(NotEqual);
	SE_Enum(GreaterEqual);
	SE_Enum(Always);
SE_END_ENUM(ComparisonFunction);

SE_BEGIN_ENUM(StencilOperation);
	SE_Enum(Keep);
	SE_Enum(Zero);
	SE_Enum(Replace);
	SE_Enum(IncrementSaturation);
	SE_Enum(DecrementSaturation);
	SE_Enum(Invert);
	SE_Enum(Increment);
	SE_Enum(Decrement);
SE_END_ENUM(StencilOperation);

SE_BEGIN_ENUM(BlendMode);
	SE_Enum(Zero);
	SE_Enum(One);
	SE_Enum(SourceColor);
	SE_Enum(InvSourceColor);
	SE_Enum(SourceAlpha);
	SE_Enum(InvSourceAlpha);
	SE_Enum(DestinationAlpha);
	SE_Enum(InvDestinationAlpha);
	SE_Enum(DestinationColor);
	SE_Enum(InvDestinationColor);
	SE_Enum(SourceAlphaSat);
	SE_Enum(BlendFactor);
	SE_Enum(InvBlendFactor);
SE_END_ENUM(BlendMode);

SE_BEGIN_ENUM(BlendOperation);
	SE_Enum(Add);
	SE_Enum(Subtract);
	SE_Enum(RevSubtract);
	SE_Enum(Min);
	SE_Enum(Max);
SE_END_ENUM(BlendOperation);

SE_BEGIN_ENUM(FogMode);
	SE_Enum(None);
	SE_Enum(Linear);
	SE_Enum(Exponential);
	SE_Enum(Exponential2);
SE_END_ENUM(FogMode);

SE_BEGIN_ENUM(TextureCoordinateMode);
	SE_Enum(PassThru);
	SE_Enum(CameraSpaceNormal);
	SE_Enum(CameraSpacePosition);
	SE_Enum(CameraSpaceReflectionVector);
	SE_Enum(SphereMap);
SE_END_ENUM(TextureCoordinateMode);

SE_BEGIN_ENUM(TextureTransform);
	SE_Enum(Disable);
	SE_Enum(Count1);
	SE_Enum(Count2);
	SE_Enum(Count3);
	SE_Enum(Count4);
	SE_Enum(Projected);
SE_END_ENUM(TextureTransform);

SE_BEGIN_ENUM(TextureOperation);
	SE_Enum(Disable);
	SE_Enum(SelectArg1);
	SE_Enum(SelectArg2);
	SE_Enum(Modulate);
	SE_Enum(Modulate2X);
	SE_Enum(Modulate4X);
	SE_Enum(Add);
	SE_Enum(AddSigned);
	SE_Enum(AddSigned2X);
	SE_Enum(Subtract);
	SE_Enum(AddSmooth);
	SE_Enum(BlendDiffuseAlpha);
	SE_Enum(BlendTextureAlpha);
	SE_Enum(BlendFactorAlpha);
	SE_Enum(BlendTextureAlphaPM);
	SE_Enum(BlendCurrentAlpha);
	SE_Enum(PreModulate);
	SE_Enum(ModulateAlphaAddColor);
	SE_Enum(ModulateColorAddAlpha);
	SE_Enum(ModulateInvAlphaAddColor);
	SE_Enum(ModulateInvColorAddAlpha);
	SE_Enum(BumpEnvironmentMap);
	SE_Enum(BumpEnvironmentMapLuminance);
	SE_Enum(DotProduct3);
	SE_Enum(MultiplyAdd);
	SE_Enum(Lerp);
SE_END_ENUM(TextureOperation);

SE_BEGIN_ENUM(TextureArgument);
	SE_Enum(Diffuse);
	SE_Enum(Current);
	SE_Enum(TextureColor);
	SE_Enum(TFactor);
	SE_Enum(Specular);
	SE_Enum(Temp);
	SE_Enum(Constant);
	SE_Enum(SelectMask);
	SE_Enum(Complement);
	SE_Enum(AlphaReplicate);
SE_END_ENUM(TextureArgument);

}

#endif

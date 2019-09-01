/*=============================================================================
GLHelper.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLHelper.h"

namespace SE_GL
{

GLint GLHelper::GetAddressMode(TextureAddressMode value)
{
	switch (value)
	{
	case TextureAddressMode_Wrap: return GL_REPEAT;
	case TextureAddressMode_Mirror: return GL_MIRRORED_REPEAT;
	case TextureAddressMode_Clamp: return GL_CLAMP_TO_EDGE;
	default: return GL_REPEAT;
	}
}

GLint GLHelper::GetFilterType(TextureFilterType value)
{
	switch (value)
	{
	case TextureFilterType_None: return GL_NEAREST;
	case TextureFilterType_Point: return GL_NEAREST;
	case TextureFilterType_Linear: return GL_LINEAR;
	case TextureFilterType_Anisotropic: return GL_LINEAR;
	default: return GL_NEAREST;
	}
}

GLint GLHelper::GetMinFilterType(TextureFilterType min, TextureFilterType mip)
{
	switch (min)
	{
	case TextureFilterType_None:
	case TextureFilterType_Point:
		switch (mip)
		{
		case TextureFilterType_None:
			return GL_NEAREST;
		case TextureFilterType_Point:
			return GL_NEAREST_MIPMAP_NEAREST;
		case TextureFilterType_Linear:
		case TextureFilterType_Anisotropic:
			return GL_NEAREST_MIPMAP_LINEAR;
		}
		break;
	case TextureFilterType_Linear:
	case TextureFilterType_Anisotropic:
		switch (mip)
		{
		case TextureFilterType_None:
			return GL_LINEAR;
		case TextureFilterType_Point:
			return GL_LINEAR_MIPMAP_NEAREST;
		case TextureFilterType_Linear:
		case TextureFilterType_Anisotropic:
			return GL_LINEAR_MIPMAP_LINEAR;
		}
		break;
	}

	return GL_NEAREST;
}

GLenum GLHelper::GetCmpFunction(ComparisonFunction value)
{
	switch (value)
	{
	case ComparisonFunction_Never: return GL_NEVER;
	case ComparisonFunction_Less: return GL_LESS;
	case ComparisonFunction_Equal: return GL_EQUAL;
	case ComparisonFunction_LessEqual: return GL_LEQUAL;
	case ComparisonFunction_Greater: return GL_GREATER;
	case ComparisonFunction_NotEqual: return GL_NOTEQUAL;
	case ComparisonFunction_GreaterEqual: return GL_GEQUAL;
	case ComparisonFunction_Always: return GL_ALWAYS;
	default: return GL_NEVER;
	}
}

GLenum GLHelper::GetStencilOp(StencilOperation value)
{
	switch (value)
	{
	case StencilOperation_Keep: return GL_KEEP;
	case StencilOperation_Zero: return GL_ZERO;
	case StencilOperation_Replace: return GL_REPLACE;
	case StencilOperation_IncrementSaturation: return GL_INCR;
	case StencilOperation_DecrementSaturation: return GL_DECR;
	case StencilOperation_Invert: return GL_NOTEQUAL;
	case StencilOperation_Increment: return GL_INCR;
	case StencilOperation_Decrement: return GL_DECR;
	default: return GL_KEEP;
	}
}

GLenum GLHelper::GetBlendMode(BlendMode value)
{
	switch (value)
	{
	case BlendMode_Zero: return GL_ZERO;
	case BlendMode_One: return GL_ONE;
	case BlendMode_SourceColor: return GL_SRC_COLOR;
	case BlendMode_InvSourceColor: return GL_ONE_MINUS_SRC_COLOR;
	case BlendMode_SourceAlpha: return GL_SRC_ALPHA;
	case BlendMode_InvSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	case BlendMode_DestinationAlpha: return GL_DST_ALPHA;
	case BlendMode_InvDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
	case BlendMode_DestinationColor: return GL_DST_COLOR;
	case BlendMode_InvDestinationColor: return GL_ONE_MINUS_DST_COLOR;
	case BlendMode_SourceAlphaSat: return GL_SRC_ALPHA_SATURATE;
	case BlendMode_BlendFactor:
	case BlendMode_InvBlendFactor:
	default:
		return GL_ONE;
	}
}

GLenum GLHelper::GetTextureOperation(TextureOperation value)
{
	switch (value)
	{
	case TextureOperation_Disable: return 0;
	case TextureOperation_SelectArg1: return GL_REPLACE;
	case TextureOperation_SelectArg2: return GL_REPLACE;
	case TextureOperation_Modulate: return GL_MODULATE;
	case TextureOperation_Modulate2X: return GL_MODULATE;
	case TextureOperation_Modulate4X: return GL_MODULATE;
	case TextureOperation_Add: return GL_ADD;
	case TextureOperation_AddSigned: return GL_ADD_SIGNED;
	case TextureOperation_AddSigned2X: return GL_ADD_SIGNED;
	case TextureOperation_Subtract: return GL_SUBTRACT;
	case TextureOperation_AddSmooth: return GL_ADD;
	case TextureOperation_BlendDiffuseAlpha: return GL_INTERPOLATE;
	case TextureOperation_BlendTextureAlpha: return GL_INTERPOLATE;
	case TextureOperation_BlendFactorAlpha: return GL_INTERPOLATE;
	case TextureOperation_BlendTextureAlphaPM: return GL_INTERPOLATE;
	case TextureOperation_BlendCurrentAlpha: return GL_INTERPOLATE;
	case TextureOperation_PreModulate: return GL_INTERPOLATE;
	case TextureOperation_ModulateAlphaAddColor: return GL_INTERPOLATE;
	case TextureOperation_ModulateColorAddAlpha: return GL_INTERPOLATE;
	case TextureOperation_ModulateInvAlphaAddColor: return GL_INTERPOLATE;
	case TextureOperation_ModulateInvColorAddAlpha: return GL_INTERPOLATE;
	case TextureOperation_BumpEnvironmentMap: return GL_INTERPOLATE;
	case TextureOperation_BumpEnvironmentMapLuminance: return GL_INTERPOLATE;
	case TextureOperation_DotProduct3: return GL_DOT3_RGB;
	case TextureOperation_MultiplyAdd: return GL_ADD;
	case TextureOperation_Lerp: return GL_INTERPOLATE;
	default: return 0;
	}
}

GLenum GLHelper::GetTextureArgument(TextureArgument value)
{
	switch (value)
	{
	case TextureArgument_Diffuse: return GL_PRIMARY_COLOR;
	case TextureArgument_current: return GL_PREVIOUS;
	case TextureArgument_textureColor: return GL_TEXTURE;
	case TextureArgument_TFactor: return GL_CONSTANT;
	case TextureArgument_Specular: return GL_PRIMARY_COLOR;
	case TextureArgument_Temp: return GL_PRIMARY_COLOR;
	case TextureArgument_Constant: return GL_CONSTANT;
	case TextureArgument_SelectMask: return GL_PRIMARY_COLOR;
	case TextureArgument_Complement: return GL_PRIMARY_COLOR;
	case TextureArgument_AlphaReplicate: return GL_PRIMARY_COLOR;
	default: return GL_PRIMARY_COLOR;
	}
}

GLenum GLHelper::GetVertexFormat(VertexFormatConstants value)
{
	switch (value)
	{
	case VertexFormat_Float1:
	case VertexFormat_Float2:
	case VertexFormat_Float3:
	case VertexFormat_Float4:
		return GL_FLOAT;
	case VertexFormat_Color:
	case VertexFormat_UByte4:
		return GL_UNSIGNED_BYTE;
	case VertexFormat_Short2:
	case VertexFormat_Short4:
		return GL_SHORT;
	default:
		return GL_UNSIGNED_BYTE;
	}
}

GLenum GLHelper::GetHardwareBufferUsage(HardwareBufferUsage value)
{
	switch (value)
	{
	case HardwareBufferUsage_Static: return GL_STATIC_DRAW_ARB;
	case HardwareBufferUsage_Dynamic: return GL_DYNAMIC_DRAW_ARB;
	default: return GL_DYNAMIC_DRAW_ARB;
	}
}

GLenum GLHelper::GetPrimitiveType(PrimitiveType value)
{
	switch (value)
	{
	case PrimitiveType_PointList: return GL_POINTS;
	case PrimitiveType_LineList: return GL_LINES;
	case PrimitiveType_LineStrip: return GL_LINE_STRIP;
	case PrimitiveType_TriangleList: return GL_TRIANGLES;
	case PrimitiveType_TriangleStrip: return GL_TRIANGLE_STRIP;
	case PrimitiveType_TriangleFan: return GL_TRIANGLE_FAN;
	default: return GL_POINTS;
	}
}

}

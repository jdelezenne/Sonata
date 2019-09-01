/*=============================================================================
D3D9Helper.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "D3D9Helper.h"

namespace SE_D3D9
{

D3DCOLOR D3D9Helper::MakeD3DColor(const Color32& value)
{
	return D3DCOLOR_COLORVALUE(
		value.R, value.G, value.B, value.A);
}

D3DXCOLOR D3D9Helper::MakeD3DXColor(const Color32& value)
{
	return D3DXCOLOR(
		value.R, value.G, value.B, value.A);
}

D3DXMATRIX D3D9Helper::MakeD3DXMatrix(const Matrix4& value)
{
	// Transpose matrix
	D3DXMATRIX result;
	result.m[0][0] = value.M[0][0];
	result.m[0][1] = value.M[1][0];
	result.m[0][2] = value.M[2][0];
	result.m[0][3] = value.M[3][0];

	result.m[1][0] = value.M[0][1];
	result.m[1][1] = value.M[1][1];
	result.m[1][2] = value.M[2][1];
	result.m[1][3] = value.M[3][1];

	result.m[2][0] = value.M[0][2];
	result.m[2][1] = value.M[1][2];
	result.m[2][2] = value.M[2][2];
	result.m[2][3] = value.M[3][2];

	result.m[3][0] = value.M[0][3];
	result.m[3][1] = value.M[1][3];
	result.m[3][2] = value.M[2][3];
	result.m[3][3] = value.M[3][3];

	return result;
}

D3DFOGMODE D3D9Helper::GetFogMode(FogMode value)
{
	D3DFOGMODE result;
	switch (value)
	{
	case FogMode_None: result = D3DFOG_NONE; break;
	case FogMode_Linear: result = D3DFOG_LINEAR; break;
	case FogMode_Exponential: result = D3DFOG_EXP; break;
	case FogMode_Exponential2: result = D3DFOG_EXP2; break;
	default: result = D3DFOG_NONE;
	}

	return result;
}

D3DTEXTUREADDRESS D3D9Helper::GetAddressMode(TextureAddressMode value)
{
	D3DTEXTUREADDRESS result;
	switch (value)
	{
	case TextureAddressMode_Wrap: result = D3DTADDRESS_WRAP; break;
	case TextureAddressMode_Mirror: result = D3DTADDRESS_MIRROR; break;
	case TextureAddressMode_Clamp: result = D3DTADDRESS_CLAMP; break;
	case TextureAddressMode_Border: result = D3DTADDRESS_BORDER; break;
	default: result = D3DTADDRESS_WRAP;
	}

	return result;
}

D3DTEXTUREFILTERTYPE D3D9Helper::GetFilterType(TextureFilterType value)
{
	D3DTEXTUREFILTERTYPE result;
	switch (value)
	{
	case TextureFilterType_None: result = D3DTEXF_NONE; break;
	case TextureFilterType_Point: result = D3DTEXF_POINT; break;
	case TextureFilterType_Linear: result = D3DTEXF_LINEAR; break;
	case TextureFilterType_Anisotropic: result = D3DTEXF_ANISOTROPIC; break;
	default: result = D3DTEXF_POINT;
	}

	return result;
}

D3DCMPFUNC D3D9Helper::GetCmpFunction(ComparisonFunction function)
{
	D3DCMPFUNC result;
	switch (function)
	{
	case ComparisonFunction_Never: result = D3DCMP_NEVER; break;
	case ComparisonFunction_Less: result = D3DCMP_LESS; break;
	case ComparisonFunction_Equal: result = D3DCMP_EQUAL; break;
	case ComparisonFunction_LessEqual: result = D3DCMP_LESSEQUAL; break;
	case ComparisonFunction_Greater: result = D3DCMP_GREATER; break;
	case ComparisonFunction_NotEqual: result = D3DCMP_NOTEQUAL; break;
	case ComparisonFunction_GreaterEqual: result = D3DCMP_GREATEREQUAL; break;
	case ComparisonFunction_Always: result = D3DCMP_ALWAYS; break;
	default: result = D3DCMP_LESSEQUAL;
	}

	return result;
}

D3DSTENCILOP D3D9Helper::GetStencilOp(StencilOperation value)
{
	D3DSTENCILOP result;
	switch (value)
	{
	case StencilOperation_Keep: result = D3DSTENCILOP_KEEP; break;
	case StencilOperation_Zero: result = D3DSTENCILOP_ZERO; break;
	case StencilOperation_Replace: result = D3DSTENCILOP_REPLACE; break;
	case StencilOperation_IncrementSaturation: result = D3DSTENCILOP_INCRSAT; break;
	case StencilOperation_DecrementSaturation: result = D3DSTENCILOP_DECRSAT; break;
	case StencilOperation_Invert: result = D3DSTENCILOP_INVERT; break;
	case StencilOperation_Increment: result = D3DSTENCILOP_INCR; break;
	case StencilOperation_Decrement: result = D3DSTENCILOP_DECR; break;
	default: result = D3DSTENCILOP_KEEP;
	}

	return result;
}

D3DBLEND D3D9Helper::GetBlendMode(BlendMode value)
{
	D3DBLEND result;
	switch (value)
	{
	case BlendMode_Zero: result = D3DBLEND_ZERO; break;
	case BlendMode_One: result = D3DBLEND_ONE; break;
	case BlendMode_SourceColor: result = D3DBLEND_SRCCOLOR; break;
	case BlendMode_InvSourceColor: result = D3DBLEND_INVSRCCOLOR; break;
	case BlendMode_SourceAlpha: result = D3DBLEND_SRCALPHA; break;
	case BlendMode_InvSourceAlpha: result = D3DBLEND_INVSRCALPHA; break;
	case BlendMode_DestinationAlpha: result = D3DBLEND_DESTALPHA; break;
	case BlendMode_InvDestinationAlpha: result = D3DBLEND_INVDESTALPHA; break;
	case BlendMode_DestinationColor: result = D3DBLEND_DESTCOLOR; break;
	case BlendMode_InvDestinationColor: result = D3DBLEND_INVDESTCOLOR; break;
	case BlendMode_SourceAlphaSat: result = D3DBLEND_SRCALPHASAT; break;
	case BlendMode_BlendFactor: result = D3DBLEND_BLENDFACTOR; break;
	case BlendMode_InvBlendFactor: result = D3DBLEND_INVBLENDFACTOR; break;
	default: result = D3DBLEND_ONE;
	}

	return result;
}

D3DBLENDOP D3D9Helper::GetBlendOperation(BlendOperation value)
{
	D3DBLENDOP result;
	switch (value)
	{
	case BlendOperation_Add: result = D3DBLENDOP_ADD; break;
	case BlendOperation_Subtract: result = D3DBLENDOP_SUBTRACT; break;
	case BlendOperation_RevSubtract: result = D3DBLENDOP_REVSUBTRACT; break;
	case BlendOperation_Min: result = D3DBLENDOP_MIN; break;
	case BlendOperation_Max: result = D3DBLENDOP_MAX; break;
	default: result = D3DBLENDOP_ADD;
	}

	return result;
}

D3DTEXTUREOP D3D9Helper::GetTextureOperation(TextureOperation value)
{
	switch (value)
	{
	case TextureOperation_Disable: return D3DTOP_DISABLE;
	case TextureOperation_SelectArg1: return D3DTOP_SELECTARG1;
	case TextureOperation_SelectArg2: return D3DTOP_SELECTARG2;
	case TextureOperation_Modulate: return D3DTOP_MODULATE;
	case TextureOperation_Modulate2X: return D3DTOP_MODULATE2X;
	case TextureOperation_Modulate4X: return D3DTOP_MODULATE4X;
	case TextureOperation_Add: return D3DTOP_ADD;
	case TextureOperation_AddSigned: return D3DTOP_ADDSIGNED;
	case TextureOperation_AddSigned2X: return D3DTOP_ADDSIGNED2X;
	case TextureOperation_Subtract: return D3DTOP_SUBTRACT;
	case TextureOperation_AddSmooth: return D3DTOP_ADDSMOOTH;
	case TextureOperation_BlendDiffuseAlpha: return D3DTOP_BLENDDIFFUSEALPHA;
	case TextureOperation_BlendTextureAlpha: return D3DTOP_BLENDTEXTUREALPHA;
	case TextureOperation_BlendFactorAlpha: return D3DTOP_BLENDFACTORALPHA;
	case TextureOperation_BlendTextureAlphaPM: return D3DTOP_BLENDTEXTUREALPHAPM;
	case TextureOperation_BlendCurrentAlpha: return D3DTOP_BLENDCURRENTALPHA;
	case TextureOperation_PreModulate: return D3DTOP_PREMODULATE;
	case TextureOperation_ModulateAlphaAddColor: return D3DTOP_MODULATEALPHA_ADDCOLOR;
	case TextureOperation_ModulateColorAddAlpha: return D3DTOP_MODULATECOLOR_ADDALPHA;
	case TextureOperation_ModulateInvAlphaAddColor: return D3DTOP_MODULATEINVALPHA_ADDCOLOR;
	case TextureOperation_ModulateInvColorAddAlpha: return D3DTOP_MODULATEINVCOLOR_ADDALPHA;
	case TextureOperation_BumpEnvironmentMap: return D3DTOP_BUMPENVMAP;
	case TextureOperation_BumpEnvironmentMapLuminance: return D3DTOP_BUMPENVMAPLUMINANCE;
	case TextureOperation_DotProduct3: return D3DTOP_DOTPRODUCT3;
	case TextureOperation_MultiplyAdd: return D3DTOP_MULTIPLYADD;
	case TextureOperation_Lerp: return D3DTOP_LERP;
	default: return D3DTOP_DISABLE;
	}
}

DWORD D3D9Helper::GetTextureArgument(TextureArgument value)
{
	switch (value)
	{
	case TextureArgument_Diffuse: return D3DTA_DIFFUSE;
	case TextureArgument_Current: return D3DTA_CURRENT;
	case TextureArgument_TextureColor: return D3DTA_TEXTURE;
	case TextureArgument_TFactor: return D3DTA_TFACTOR;
	case TextureArgument_Specular: return D3DTA_SPECULAR;
	case TextureArgument_Temp: return D3DTA_TEMP;
	case TextureArgument_Constant: return D3DTA_CONSTANT;
	case TextureArgument_SelectMask: return D3DTA_SELECTMASK;
	case TextureArgument_Complement: return D3DTA_COMPLEMENT;
	case TextureArgument_AlphaReplicate: return D3DTA_ALPHAREPLICATE;
	default: return D3DTA_DIFFUSE;
	}
}

DWORD D3D9Helper::GetTextureCoordinateMode(TextureCoordinateMode value)
{
	switch (value)
	{
	case TextureCoordinateMode_PassThru: return D3DTSS_TCI_PASSTHRU;
	case TextureCoordinateMode_CameraSpaceNormal: return D3DTSS_TCI_CAMERASPACENORMAL;
	case TextureCoordinateMode_CameraSpacePosition: return D3DTSS_TCI_CAMERASPACEPOSITION;
	case TextureCoordinateMode_CameraSpaceReflectionVector: return D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;
	case TextureCoordinateMode_SphereMap: return D3DTSS_TCI_SPHEREMAP;
	default: return D3DTSS_TCI_PASSTHRU;
	}
}

D3DTEXTURETRANSFORMFLAGS D3D9Helper::GetTextureTransform(TextureTransform value)
{
	switch (value)
	{
	case TextureTransform_Disable: return D3DTTFF_DISABLE;
	case TextureTransform_Count1: return D3DTTFF_COUNT1;
	case TextureTransform_Count2: return D3DTTFF_COUNT2;
	case TextureTransform_Count3: return D3DTTFF_COUNT3;
	case TextureTransform_Count4: return D3DTTFF_COUNT4;
	case TextureTransform_Projected: return D3DTTFF_PROJECTED;
	default: return D3DTTFF_DISABLE;
	}
}

D3DDECLTYPE D3D9Helper::GetVertexFormatType(VertexFormat value)
{
	D3DDECLTYPE result;
	switch (value)
	{
	case VertexFormat_Float1: result = D3DDECLTYPE_FLOAT1; break;
	case VertexFormat_Float2: result = D3DDECLTYPE_FLOAT2; break;
	case VertexFormat_Float3: result = D3DDECLTYPE_FLOAT3; break;
	case VertexFormat_Float4: result = D3DDECLTYPE_FLOAT4; break;
	case VertexFormat_Color: result = D3DDECLTYPE_D3DCOLOR; break;
	case VertexFormat_UByte4: result = D3DDECLTYPE_UBYTE4; break;
	case VertexFormat_Short2: result = D3DDECLTYPE_SHORT2; break;
	case VertexFormat_Short4: result = D3DDECLTYPE_SHORT4; break;
	case VertexFormat_UByte4N: result = D3DDECLTYPE_UBYTE4N; break;
	default: result = D3DDECLTYPE_FLOAT1;
	}

	return result;
}

D3DDECLUSAGE D3D9Helper::GetVertexSemantic(VertexSemantic value)
{
	D3DDECLUSAGE result;
	switch (value)
	{
	case VertexSemantic_Position: result = D3DDECLUSAGE_POSITION; break;
	case VertexSemantic_BlendWeight: result = D3DDECLUSAGE_BLENDWEIGHT; break;
	case VertexSemantic_BlendIndices: result = D3DDECLUSAGE_BLENDINDICES; break;
	case VertexSemantic_Normal: result = D3DDECLUSAGE_NORMAL; break;
	case VertexSemantic_PSize: result = D3DDECLUSAGE_PSIZE; break;
	case VertexSemantic_TextureCoordinate: result = D3DDECLUSAGE_TEXCOORD; break;
	case VertexSemantic_Tangent: result = D3DDECLUSAGE_TANGENT; break;
	case VertexSemantic_Binormal: result = D3DDECLUSAGE_BINORMAL; break;
	case VertexSemantic_TesselateFactor: result = D3DDECLUSAGE_TESSFACTOR; break;
	case VertexSemantic_PositionTransformed: result = D3DDECLUSAGE_POSITIONT; break;
	case VertexSemantic_Color: result = D3DDECLUSAGE_COLOR; break;
	case VertexSemantic_Fog: result = D3DDECLUSAGE_FOG; break;
	case VertexSemantic_Depth: result = D3DDECLUSAGE_DEPTH; break;
	case VertexSemantic_Sample3: result = D3DDECLUSAGE_SAMPLE; break;
	default: result = D3DDECLUSAGE_POSITION;
	}

	return result;
}

D3DFORMAT D3D9Helper::GetFormat(PixelFormat value)
{
	/*if (value.GetSpecial() == PixelFormatSpecial_Unknown)
		return D3DFMT_UNKNOWN;
	if (value.GetSpecial() == PixelFormatSpecial_DXT1)
		return D3DFMT_DXT1;
	if (value.GetSpecial() == PixelFormatSpecial_DXT2)
		return D3DFMT_DXT2;
	if (value.GetSpecial() == PixelFormatSpecial_DXT3)
		return D3DFMT_DXT3;
	if (value.GetSpecial() == PixelFormatSpecial_DXT4)
		return D3DFMT_DXT4;
	if (value.GetSpecial() == PixelFormatSpecial_DXT5)
		return D3DFMT_DXT5;

	if (value == PixelFormat::R8G8B8)
		return D3DFMT_R8G8B8;
	if (value == PixelFormat::A8R8G8B8)
		return D3DFMT_A8R8G8B8;
	if (value == PixelFormat::X8R8G8B8)
		return D3DFMT_X8R8G8B8;
	if (value == PixelFormat::R5G6B5)
		return D3DFMT_R5G6B5;
	if (value == PixelFormat::X1R5G5B5)
		return D3DFMT_X1R5G5B5;
	if (value == PixelFormat::A1R5G5B5)
		return D3DFMT_A1R5G5B5;
	if (value == PixelFormat::A4R4G4B4)
		return D3DFMT_A4R4G4B4;
	if (value == PixelFormat::X4R4G4B4)
		return D3DFMT_X4R4G4B4;
	if (value == PixelFormat::A8B8G8R8)
		return D3DFMT_A8B8G8R8;
	if (value == PixelFormat::X8B8G8R8)
		return D3DFMT_X8B8G8R8;*/

	switch (value)
	{
	case PixelFormat_Unknown:
		return D3DFMT_UNKNOWN;
	case PixelFormat_R8G8B8:
		return D3DFMT_R8G8B8;
	case PixelFormat_R8G8B8A8:
		return D3DFMT_A8R8G8B8;
	case PixelFormat_DXT1:
		return D3DFMT_DXT1;
	case PixelFormat_DXT3:
		return D3DFMT_DXT3;
	case PixelFormat_DXT5:
		return D3DFMT_DXT5;
	default:
		return D3DFMT_UNKNOWN;
	}
}

int32 D3D9Helper::GetColorChannelBits(D3DFORMAT format)
{
	switch (format)
	{
	case D3DFMT_R8G8B8:
		return 8;
	case D3DFMT_A8R8G8B8:
		return 8;
	case D3DFMT_X8R8G8B8:
		return 8;
	case D3DFMT_R5G6B5:
		return 5;
	case D3DFMT_X1R5G5B5:
		return 5;
	case D3DFMT_A1R5G5B5:
		return 5;
	case D3DFMT_A4R4G4B4:
		return 4;
	case D3DFMT_R3G3B2:
		return 2;
	case D3DFMT_A8R3G3B2:
		return 2;
	case D3DFMT_X4R4G4B4:
		return 4;
	case D3DFMT_A2B10G10R10:
		return 10;
	case D3DFMT_A8B8G8R8:
		return 8;
	case D3DFMT_A2R10G10B10:
		return 10;
	case D3DFMT_A16B16G16R16:
		return 16;
	default:
		return 0;
	}
}

int32 D3D9Helper::GetAlphaChannelBits(D3DFORMAT format)
{
	switch (format)
	{
		case D3DFMT_R8G8B8:
			return 0;
		case D3DFMT_A8R8G8B8:
			return 8;
		case D3DFMT_X8R8G8B8:
			return 0;
		case D3DFMT_R5G6B5:
			return 0;
		case D3DFMT_X1R5G5B5:
			return 0;
		case D3DFMT_A1R5G5B5:
			return 1;
		case D3DFMT_A4R4G4B4:
			return 4;
		case D3DFMT_R3G3B2:
			return 0;
		case D3DFMT_A8R3G3B2:
			return 8;
		case D3DFMT_X4R4G4B4:
			return 0;
		case D3DFMT_A2B10G10R10:
			return 2;
		case D3DFMT_A8B8G8R8:
			return 8;
		case D3DFMT_A2R10G10B10:
			return 2;
		case D3DFMT_A16B16G16R16:
			return 16;
		default:
			return 0;
	}
}

int32 D3D9Helper::GetDepthBits(D3DFORMAT format)
{
	switch (format)
	{
		case D3DFMT_D32F_LOCKABLE:
		case D3DFMT_D32:
			return 32;

		case D3DFMT_D24X8:
		case D3DFMT_D24S8:
		case D3DFMT_D24X4S4:
		case D3DFMT_D24FS8:
			return 24;

		case D3DFMT_D16_LOCKABLE:
		case D3DFMT_D16:
			return 16;

		case D3DFMT_D15S1:
			return 15;

		default:
			return 0;
	}
}

int32 D3D9Helper::GetStencilBits(D3DFORMAT format)
{
	switch (format)
	{
		case D3DFMT_D16_LOCKABLE:
		case D3DFMT_D16:
		case D3DFMT_D32F_LOCKABLE:
		case D3DFMT_D32:
		case D3DFMT_D24X8:
			return 0;

		case D3DFMT_D15S1:
			return 1;

		case D3DFMT_D24X4S4:
			return 4;

		case D3DFMT_D24S8:
		case D3DFMT_D24FS8:
			return 8;

		default:
			return 0;
	}
}

int32 D3D9Helper::GetBitDepth(D3DFORMAT format)
{
	switch (format)
	{
	case D3DFMT_R8G8B8:
	case D3DFMT_R5G6B5:
	case D3DFMT_R3G3B2:
		return 3;

	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_A4R4G4B4:
	case D3DFMT_A8R3G3B2:
	case D3DFMT_X4R4G4B4:
	case D3DFMT_A2B10G10R10:
	case D3DFMT_A8B8G8R8:
	case D3DFMT_A2R10G10B10:
	case D3DFMT_A16B16G16R16:
		return 4;

	default:
		return 0;
	}
}

D3DXPARAMETER_TYPE D3D9Helper::GetParameterType(ShaderParameterType value)
{
	switch (value)
	{
	case ShaderParameterType_Void: return D3DXPT_VOID;
	case ShaderParameterType_Boolean: return D3DXPT_BOOL;
	case ShaderParameterType_Integer: return D3DXPT_INT;
	case ShaderParameterType_Float: return D3DXPT_FLOAT;
	case ShaderParameterType_String: return D3DXPT_STRING;
	case ShaderParameterType_Texture: return D3DXPT_TEXTURE;
	case ShaderParameterType_Texture1D: return D3DXPT_TEXTURE1D;
	case ShaderParameterType_Texture2D: return D3DXPT_TEXTURE2D;
	case ShaderParameterType_Texture3D: return D3DXPT_TEXTURE3D;
	case ShaderParameterType_TextureCube: return D3DXPT_TEXTURECUBE;
	case ShaderParameterType_Sampler: return D3DXPT_SAMPLER;
	case ShaderParameterType_Sampler1D: return D3DXPT_SAMPLER1D;
	case ShaderParameterType_Sampler2D: return D3DXPT_SAMPLER2D;
	case ShaderParameterType_Sampler3D: return D3DXPT_SAMPLER3D;
	case ShaderParameterType_SamplerCube: return D3DXPT_SAMPLERCUBE;
	case ShaderParameterType_PixelShader: return D3DXPT_PIXELSHADER;
	case ShaderParameterType_VertexShader: return D3DXPT_VERTEXSHADER;
	case ShaderParameterType_GeometryShader: return D3DXPT_VOID;
	default: return D3DXPT_VOID;
	}
}

ShaderParameterType D3D9Helper::GetParameterType(D3DXPARAMETER_TYPE value)
{
	switch (value)
	{
	case D3DXPT_VOID: return ShaderParameterType_Void;
	case D3DXPT_BOOL: return ShaderParameterType_Boolean;
	case D3DXPT_INT: return ShaderParameterType_Integer;
	case D3DXPT_FLOAT: return ShaderParameterType_Float;
	case D3DXPT_STRING: return ShaderParameterType_String;
	case D3DXPT_TEXTURE: return ShaderParameterType_Texture;
	case D3DXPT_TEXTURE1D: return ShaderParameterType_Texture1D;
	case D3DXPT_TEXTURE2D: return ShaderParameterType_Texture2D;
	case D3DXPT_TEXTURE3D: return ShaderParameterType_Texture3D;
	case D3DXPT_TEXTURECUBE: return ShaderParameterType_TextureCube;
	case D3DXPT_SAMPLER: return ShaderParameterType_Sampler;
	case D3DXPT_SAMPLER1D: return ShaderParameterType_Sampler1D;
	case D3DXPT_SAMPLER2D: return ShaderParameterType_Sampler2D;
	case D3DXPT_SAMPLER3D: return ShaderParameterType_Sampler3D;
	case D3DXPT_SAMPLERCUBE: return ShaderParameterType_SamplerCube;
	case D3DXPT_PIXELSHADER: return ShaderParameterType_PixelShader;
	case D3DXPT_VERTEXSHADER: return ShaderParameterType_VertexShader;
	default: return ShaderParameterType_Void;
	}
}

D3DXPARAMETER_CLASS D3D9Helper::GetParameterClass(ShaderParameterClass value)
{
	switch (value)
	{
	case ShaderParameterClass_Scalar: return D3DXPC_SCALAR;
	case ShaderParameterClass_Vector: return D3DXPC_VECTOR;
	case ShaderParameterClass_Matrix_Rows: return D3DXPC_MATRIX_ROWS;
	case ShaderParameterClass_Matrix_Columns: return D3DXPC_MATRIX_COLUMNS;
	case ShaderParameterClass_Object: return D3DXPC_OBJECT;
	case ShaderParameterClass_Struct: return D3DXPC_STRUCT;
	default: return D3DXPC_OBJECT;
	}
}

ShaderParameterClass D3D9Helper::GetParameterClass(D3DXPARAMETER_CLASS value)
{
	switch (value)
	{
	case D3DXPC_SCALAR: return ShaderParameterClass_Scalar;
	case D3DXPC_VECTOR: return ShaderParameterClass_Vector;
	case D3DXPC_MATRIX_ROWS: return ShaderParameterClass_Matrix_Rows;
	case D3DXPC_MATRIX_COLUMNS: return ShaderParameterClass_Matrix_Columns;
	case D3DXPC_OBJECT: return ShaderParameterClass_Object;
	case D3DXPC_STRUCT: return ShaderParameterClass_Struct;
	default: return ShaderParameterClass_Object;
	}
}

}

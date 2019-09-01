/*=============================================================================
D3D9Helper.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9HELPER_H_
#define _SE_D3D9HELPER_H_

#include "D3D9RenderSystem.h"

namespace SE_D3D9
{

/** D3D9 helper class. */
class D3D9Helper
{
public:
	static D3DCOLOR MakeD3DColor(const Color32& value);

	static D3DXCOLOR MakeD3DXColor(const Color32& value);

	static D3DXMATRIX MakeD3DXMatrix(const Matrix4& value);

	static D3DFOGMODE GetFogMode(FogMode value);

	static D3DTEXTUREADDRESS GetAddressMode(TextureAddressMode value);

	static D3DTEXTUREFILTERTYPE GetFilterType(TextureFilterType value);

	static D3DCMPFUNC GetCmpFunction(ComparisonFunction value);

	static D3DSTENCILOP GetStencilOp(StencilOperation value);

	static D3DBLEND GetBlendMode(BlendMode value);

	static D3DBLENDOP GetBlendOperation(BlendOperation value);

	static D3DTEXTUREOP GetTextureOperation(TextureOperation value);

	static DWORD GetTextureArgument(TextureArgument value);

	static DWORD GetTextureCoordinateMode(TextureCoordinateMode value);

	static D3DTEXTURETRANSFORMFLAGS GetTextureTransform(TextureTransform value);

	static D3DDECLTYPE GetVertexFormatType(VertexFormat value);

	static D3DDECLUSAGE GetVertexSemantic(VertexSemantic value);

	static D3DFORMAT GetFormat(PixelFormat value);

	static int32 GetColorChannelBits(D3DFORMAT format);

	static int32 GetAlphaChannelBits(D3DFORMAT format);

	static int32 GetDepthBits(D3DFORMAT format);

	static int32 GetStencilBits(D3DFORMAT format);

	static int32 GetBitDepth(D3DFORMAT format);

	static D3DXPARAMETER_TYPE GetParameterType(ShaderParameterType value);

	static ShaderParameterType GetParameterType(D3DXPARAMETER_TYPE value);

	static D3DXPARAMETER_CLASS GetParameterClass(ShaderParameterClass value);

	static ShaderParameterClass GetParameterClass(D3DXPARAMETER_CLASS value);
};

}

#endif 

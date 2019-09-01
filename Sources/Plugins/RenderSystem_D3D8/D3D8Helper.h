/*=============================================================================
D3D8Helper.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8HELPER_H_
#define _SE_D3D8HELPER_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

/** D3D8 helper class. */
class D3D8Helper
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

	static DWORD GetVertexFormatType(VertexFormatConstants value);

	static DWORD GetVertexSemantic(VertexSemanticConstants value);

	static D3DFORMAT GetFormat(PixelFormat value);

	static int32 GetColorChannelBits(D3DFORMAT format);

	static int32 GetAlphaChannelBits(D3DFORMAT format);

	static int32 GetDepthBits(D3DFORMAT format);

	static int32 GetStencilBits(D3DFORMAT format);

	static int32 GetBitDepth(D3DFORMAT format);
};

}

#endif 

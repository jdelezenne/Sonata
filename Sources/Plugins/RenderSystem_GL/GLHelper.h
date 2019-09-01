/*=============================================================================
GLHelper.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLHELPER_H_
#define _SE_GLHELPER_H_

#include "GLRenderSystem.h"

namespace SE_GL
{

/** OpenGL helper class. */
class GLHelper
{
public:
	static GLint GetAddressMode(TextureAddressMode value);

	static GLint GetFilterType(TextureFilterType value);

	static GLint GetMinFilterType(TextureFilterType min, TextureFilterType mip);

	static GLenum GetCmpFunction(ComparisonFunction value);

	static GLenum GetStencilOp(StencilOperation value);

	static GLenum GetBlendMode(BlendMode value);

	static GLenum GetTextureOperation(TextureOperation value);

	static GLenum GetTextureArgument(TextureArgument value);

	static GLenum GetVertexFormat(VertexFormatConstants value);

	static GLenum GetHardwareBufferUsage(HardwareBufferUsage value);

	static GLenum GetPrimitiveType(PrimitiveType value);
};

}

#endif 

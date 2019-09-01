/*=============================================================================
PixelShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PIXELSHADERPROGRAM_H_
#define _SE_PIXELSHADERPROGRAM_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/ShaderProgram.h"

namespace SonataEngine
{

/**
	@brief Base class for the Pixel Shader Programs.
*/
class SE_GRAPHICS_EXPORT PixelShaderProgram : public ShaderProgram
{
public:
	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Pixel; }
};

SEPointer(PixelShaderProgram);

}

#endif 

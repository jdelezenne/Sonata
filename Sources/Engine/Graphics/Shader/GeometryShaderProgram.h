/*=============================================================================
GeometryShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GEOMETRYSHADERPROGRAM_H_
#define _SE_GEOMETRYSHADERPROGRAM_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/ShaderProgram.h"

namespace SonataEngine
{

/**
	@brief Base class for the Geometry Shader Programs.
*/
class SE_GRAPHICS_EXPORT GeometryShaderProgram : public ShaderProgram
{
public:
	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Geometry; }
};

SEPointer(GeometryShaderProgram);

}

#endif 

/*=============================================================================
VertexShaderProgram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXSHADERPROGRAM_H_
#define _SE_VERTEXSHADERPROGRAM_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/ShaderProgram.h"

namespace SonataEngine
{

/**
	@brief Base class for the Vertex Shader Programs.
*/
class SE_GRAPHICS_EXPORT VertexShaderProgram : public ShaderProgram
{
public:
	virtual ShaderProgramType GetProgramType() const { return ShaderProgramType_Vertex; }
};

SEPointer(VertexShaderProgram);

}

#endif 

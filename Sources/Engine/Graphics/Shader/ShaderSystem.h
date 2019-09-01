/*=============================================================================
ShaderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SHADERSYSTEM_H_
#define _SE_SHADERSYSTEM_H_

#include "Core/Object.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/Shader/EffectShader.h"

namespace SonataEngine
{

/**
	@brief Shader System.

	Base class for shader system implementations.
	A shader system is responsible for the creation and binding of shader programs,
	and the creation of effect shaders from source files.
*/
class SE_GRAPHICS_EXPORT ShaderSystem : public Manager, public Context<ShaderSystem>
{
public:
	virtual ~ShaderSystem();

	virtual ShaderProgram* CreateShaderProgram(ShaderProgramType type) = 0;

	virtual void DestroyShaderProgram(ShaderProgram* program) = 0;

	virtual bool SetShaderProgram(ShaderProgram* program) = 0;

	virtual bool DisableShaderProgram(ShaderProgram* program) = 0;

	virtual EffectShader* CreateEffectShader(const String& source) = 0;

	virtual void DestroyEffectShader(EffectShader* shader) = 0;

protected:
	ShaderSystem();
};

SEPointer(ShaderSystem);

}

#endif 

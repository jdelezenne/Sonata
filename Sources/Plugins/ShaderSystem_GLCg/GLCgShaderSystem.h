/*=============================================================================
GLCgShaderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLCGSHADERSYSTEM_H_
#define _SE_GLCGSHADERSYSTEM_H_

#include <Cg/Cg.h>
#include <Cg/CgGL.h>

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_GLCg
{

bool cgCheckError();

/**
	OpenGL Cg Shader System.

	@todo Handle GL context change.
*/
class GLCgShaderSystem : public ShaderSystem
{
public:
	GLCgShaderSystem();
	virtual ~GLCgShaderSystem();

	virtual	bool Create();

	virtual	void Destroy();

	virtual void Update(real64 elapsed);

	virtual ShaderProgram* CreateShaderProgram(ShaderProgramType type);

	virtual void DestroyShaderProgram(ShaderProgram* program);

	virtual bool SetShaderProgram(ShaderProgram* program);

	virtual bool DisableShaderProgram(ShaderProgram* program);

	CGcontext GetCGContext() const { return _CGContext; }
	CGprofile GetCGVertexProfile() const { return _CGVertexProfile; }
	CGprofile GetCGPixelProfile() const { return _CGPixelProfile; }

protected:
	CGcontext _CGContext;
	CGprofile _CGVertexProfile;
	CGprofile _CGPixelProfile;
};

}

#endif

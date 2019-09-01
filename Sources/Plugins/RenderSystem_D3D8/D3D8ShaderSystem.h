/*=============================================================================
D3D8ShaderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8SHADERSYSTEM_H_
#define _SE_D3D8SHADERSYSTEM_H_

#include "D3D8RenderSystem.h"

namespace SE_D3D8
{

class D3D8ShaderSystem : public ShaderSystem
{
public:
	D3D8ShaderSystem(IDirect3DDevice8* pD3DDevice);
	virtual ~D3D8ShaderSystem();

	virtual	bool Create();

	virtual	void Destroy();

	virtual void Update(real64 elapsed);

	virtual ShaderProgram* CreateShaderProgram(ShaderProgramType type);

	virtual void DestroyShaderProgram(ShaderProgram* program);

	virtual bool SetShaderProgram(ShaderProgram* program);

	virtual bool DisableShaderProgram(ShaderProgram* program);

	virtual ShaderMaterial* CreateShader(Stream& stream);

protected:
	IDirect3DDevice8* _D3DDevice;
};

}

#endif

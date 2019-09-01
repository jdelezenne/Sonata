/*=============================================================================
D3D8CgShaderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D8CGSHADERSYSTEM_H_
#define _SE_D3D8CGSHADERSYSTEM_H_

#include <Cg/Cg.h>
#include <Cg/CgD3D8.h>

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_D3D8Cg
{

/**
	Direct3D8 Cg Shader System.

	@todo Handle D3D8 device reset/change.
*/
class D3D8CgShaderSystem : public ShaderSystem
{
public:
	D3D8CgShaderSystem(IDirect3DDevice8* pD3DDevice);
	virtual ~D3D8CgShaderSystem();

	virtual	bool Create();

	virtual	void Destroy();

	virtual void Update(real64 elapsed);

	virtual ShaderProgram* CreateShaderProgram(ShaderProgramType type);

	virtual void DestroyShaderProgram(ShaderProgram* program);

	virtual bool SetShaderProgram(ShaderProgram* program);

	virtual bool DisableShaderProgram(ShaderProgram* program);

	IDirect3DDevice8* GetD3DDevice() const { return _D3DDevice; }
	CGcontext GetCGContext() const { return _CGContext; }
	CGprofile GetCGVertexProfile() const { return _CGVertexProfile; }
	CGprofile GetCGPixelProfile() const { return _CGPixelProfile; }

protected:
	IDirect3DDevice8* _D3DDevice;
	CGcontext _CGContext;
	CGprofile _CGVertexProfile;
	CGprofile _CGPixelProfile;
};

}

#endif

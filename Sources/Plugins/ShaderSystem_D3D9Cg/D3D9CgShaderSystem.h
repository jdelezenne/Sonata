/*=============================================================================
D3D9CgShaderSystem.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9CGSHADERSYSTEM_H_
#define _SE_D3D9CGSHADERSYSTEM_H_

#include <Cg/Cg.h>
#include <Cg/CgD3D9.h>

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_D3D9Cg
{

/**
	Direct3D9 Cg Shader System.

	@todo Handle D3D9 device reset/change.
*/
class D3D9CgShaderSystem : public ShaderSystem
{
public:
	D3D9CgShaderSystem(IDirect3DDevice9* pD3DDevice);
	virtual ~D3D9CgShaderSystem();

	virtual	bool Create();

	virtual	void Destroy();

	virtual void Update(real64 elapsed);

	virtual ShaderProgram* CreateShaderProgram(ShaderProgramType type);

	virtual void DestroyShaderProgram(ShaderProgram* program);

	virtual bool SetShaderProgram(ShaderProgram* program);

	virtual bool DisableShaderProgram(ShaderProgram* program);

	virtual ShaderMaterial* CreateShader(Stream& stream);

	virtual void DestroyShader(ShaderMaterial* shader);

	IDirect3DDevice9* GetD3DDevice() const { return _D3DDevice; }
	CGcontext GetCGContext() const { return _CGContext; }
	CGprofile GetCGVertexProfile() const { return _CGVertexProfile; }
	CGprofile GetCGPixelProfile() const { return _CGPixelProfile; }

protected:
	IDirect3DDevice9* _D3DDevice;
	CGcontext _CGContext;
	CGprofile _CGVertexProfile;
	CGprofile _CGPixelProfile;
};

}

#endif

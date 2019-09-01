/*=============================================================================
D3D9EffectPass.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9EFFECTPASS_H_
#define _SE_D3D9EFFECTPASS_H_

#include "D3D9HLSLShaderSystem.h"
#include "D3D9EffectShader.h"
#include "D3D9HLSLShaderProgram.h"
#include "D3D9EffectAnnotation.h"

namespace SE_D3D9
{

/** Direct3D9 Effect Pass. */
class D3D9EffectPass : public D3D9EffectVariable, public EffectPass
{
public:
	typedef Array<D3D9EffectAnnotation*> AnnotationList;

protected:
	D3D9HLSLVertexShaderProgram* _vertexProgram;
	D3D9HLSLPixelShaderProgram* _pixelProgram;
	AnnotationList _annotations;

public:
	D3D9EffectPass(D3D9EffectShader* shader, D3DXHANDLE handle);
	virtual ~D3D9EffectPass();

	virtual ShaderProgram* GetVertexProgram() const;
	virtual void SetVertexProgram(ShaderProgram* value);

	virtual ShaderProgram* GetPixelProgram() const;
	virtual void SetPixelProgram(ShaderProgram* value);

	virtual ShaderProgram* GetGeometryProgram() const;
	virtual void SetGeometryProgram(ShaderProgram* value);

	virtual int GetAnnotationCount() const;
	virtual EffectAnnotation* GetAnnotationByIndex(int index) const;
	virtual EffectAnnotation* GetAnnotationByName(const String& name) const;

protected:
	void Initialize();
};

}

#endif

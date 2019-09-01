/*=============================================================================
D3D9EffectTechnique.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9EFFECTTECHNIQUE_H_
#define _SE_D3D9EFFECTTECHNIQUE_H_

#include "D3D9RenderSystem.h"
#include "D3D9EffectShader.h"
#include "D3D9EffectPass.h"
#include "D3D9EffectAnnotation.h"

namespace SE_D3D9
{

/** Direct3D9 Effect Technique. */
class D3D9EffectTechnique : public D3D9EffectVariable, public EffectTechnique
{
public:
	typedef Array<D3D9EffectPass*> PassList;
	typedef Array<D3D9EffectAnnotation*> AnnotationList;

protected:
	PassList _passes;
	AnnotationList _annotations;
	D3D9EffectPass* _activePass;

public:
	D3D9EffectTechnique(D3D9EffectShader* shader, D3DXHANDLE handle);
	virtual ~D3D9EffectTechnique();

	virtual int GetPassCount() const;
	virtual EffectPass* GetPassByIndex(int index) const;
	virtual EffectPass* GetPassByName(const String& name) const;

	virtual int GetAnnotationCount() const;
	virtual EffectAnnotation* GetAnnotationByIndex(int index) const;
	virtual EffectAnnotation* GetAnnotationByName(const String& name) const;

	virtual void BeginPass(EffectPass* pass);
	virtual void BeginPassFromIndex(int index);
	virtual EffectPass* GetActivePass();
	virtual void EndPass();

protected:
	void Initialize();
};

}

#endif

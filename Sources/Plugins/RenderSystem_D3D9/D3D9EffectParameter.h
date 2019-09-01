/*=============================================================================
D3D9EffectParameter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9EFFECTPARAMETER_H_
#define _SE_D3D9EFFECTPARAMETER_H_

#include "D3D9HLSLShaderSystem.h"
#include "D3D9EffectShader.h"
#include "D3D9EffectAnnotation.h"

namespace SE_D3D9
{

/** Direct3D9 Effect Parameter. */
class D3D9EffectParameter : public D3D9EffectVariable, public EffectParameter
{
public:
	typedef Array<D3D9EffectParameter*> ParameterList;
	typedef Array<D3D9EffectAnnotation*> AnnotationList;

protected:
	ParameterList _members;
	AnnotationList _annotations;

public:
	D3D9EffectParameter(D3D9EffectShader* effect, D3DXHANDLE handle);
	virtual ~D3D9EffectParameter();

	virtual EffectParameter* GetMemberByIndex(int index) const;
	virtual EffectParameter* GetMemberByName(const String& name) const;
	virtual EffectParameter* GetMemberBySemantic(const String& semantic) const;

	virtual bool GetValue(void** value);
	virtual bool SetValue(void* value);

	virtual int GetAnnotationCount() const;
	virtual EffectAnnotation* GetAnnotationByIndex(int index) const;
	virtual EffectAnnotation* GetAnnotationByName(const String& name) const;

protected:
	void Initialize();
};

}

#endif

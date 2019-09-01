/*=============================================================================
D3D9EffectAnnotation.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_D3D9EFFECTANNOTATION_H_
#define _SE_D3D9EFFECTANNOTATION_H_

#include "D3D9HLSLShaderSystem.h"
#include "D3D9EffectShader.h"

namespace SE_D3D9
{

/** Direct3D9 Effect Annotation. */
class D3D9EffectAnnotation : public D3D9EffectVariable, public EffectAnnotation
{
public:
	D3D9EffectAnnotation(D3D9EffectShader* shader, D3DXHANDLE handle);
	virtual ~D3D9EffectAnnotation();

	virtual bool GetValueBoolean();
	virtual int32 GetValueInt32();
	virtual Matrix4 GetValueMatrix();
	virtual real32 GetValueSingle();
	virtual String GetValueString();
	virtual Vector2 GetValueVector2();
	virtual Vector3 GetValueVector3();
	virtual Vector4 GetValueVector4();

protected:
	void Initialize();
	bool GetValue(void* value);
};

}

#endif

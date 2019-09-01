/*=============================================================================
EffectAnnotation.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EFFECTANNOTATION_H_
#define _SE_EFFECTANNOTATION_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/ShaderParameterDesc.h"

namespace SonataEngine
{

/**
	@brief Effect Shader Annotation.
*/
class SE_GRAPHICS_EXPORT EffectAnnotation : public NamedObject
{
protected:
	ShaderParameterDesc _parameterDesc;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	EffectAnnotation();

	/** Destructor. */
	virtual ~EffectAnnotation();
	//@}

	/** @name Properties. */
	//@{
	const ShaderParameterDesc& GetParameterDesc() const { return _parameterDesc; }

	/** Reads the memory occupied by the annotation. */
	virtual bool GetValueBoolean() = 0;
	virtual int32 GetValueInt32() = 0;
	virtual Matrix4 GetValueMatrix() = 0;
	virtual real32 GetValueSingle() = 0;
	virtual String GetValueString() = 0;
	virtual Vector2 GetValueVector2() = 0;
	virtual Vector3 GetValueVector3() = 0;
	virtual Vector4 GetValueVector4() = 0;

	//@}
};

SEPointer(EffectAnnotation);

}

#endif

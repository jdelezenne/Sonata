/*=============================================================================
EffectParameter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EFFECTPARAMETER_H_
#define _SE_EFFECTPARAMETER_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/EffectAnnotation.h"
#include "Graphics/Shader/ShaderParameterDesc.h"

namespace SonataEngine
{

/**
	@brief Effect Shader Parameter.
*/
class SE_GRAPHICS_EXPORT EffectParameter : public NamedObject
{
protected:
	ShaderParameterDesc _parameterDesc;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	EffectParameter();

	/** Destructor. */
	virtual ~EffectParameter();
	//@}

	/** @name Properties. */
	//@{
	const ShaderParameterDesc& GetParameterDesc() const { return _parameterDesc; }

	/** Gets a member of a structure. */
	virtual EffectParameter* GetMemberByIndex(int index) const = 0;

	/** Gets a member of a structure. */
	virtual EffectParameter* GetMemberByName(const String& name) const = 0;

	/** Gets a member of a structure. */
	virtual EffectParameter* GetMemberBySemantic(const String& semantic) const = 0;

	/** Reads the memory occupied by the variable. */
	virtual bool GetValue(void** value) = 0;

	/** Writes to the memory occupied by the variable. */
	virtual bool SetValue(void* value) = 0;
	//@}

	/** @name Annotations. */
	//@{
	virtual int GetAnnotationCount() const = 0;
	virtual EffectAnnotation* GetAnnotationByIndex(int index) const = 0;
	virtual EffectAnnotation* GetAnnotationByName(const String& name) const = 0;
	//@}
};

}

#endif

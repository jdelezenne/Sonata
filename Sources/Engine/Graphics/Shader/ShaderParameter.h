/*=============================================================================
ShaderParameter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SHADERPARAMETER_H_
#define _SE_SHADERPARAMETER_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/ShaderParameterDesc.h"

namespace SonataEngine
{

/** Shader parameter variability. */
enum ShaderParameterVariability
{
	/** A uniform parameter is one whose value does not change with each invocation of a program, but whose value can change between groups of program invocations. */
	ShaderParameterVariability_Uniform,

	/** A literal parameter is folded out at compile time. */
	ShaderParameterVariability_Literal
};

/**
	@brief Shader parameter.
*/
class SE_GRAPHICS_EXPORT ShaderParameter : public NamedObject
{
protected:
	ShaderParameterDesc _parameterDesc;
	ShaderParameterVariability _parameterVariability;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	ShaderParameter();

	/** Destructor. */
	virtual ~ShaderParameter();
	//@}

	/** @name Properties. */
	//@{
	const ShaderParameterDesc& GetParameterDesc() const { return _parameterDesc; }

	ShaderParameterVariability GetParameterVariability() const { return _parameterVariability; }

	/** Gets a member of a structure. */
	virtual ShaderParameter* GetMemberByIndex(int index) const = 0;

	/** Gets a member of a structure. */
	virtual ShaderParameter* GetMemberByName(const String& name) const = 0;

	/** Writes to the memory occupied by the variable. */
	virtual bool SetValue(void* value) = 0;
	//@}
};

}

#endif

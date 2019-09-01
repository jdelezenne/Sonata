/*=============================================================================
EffectShader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EFFECTSHADER_H_
#define _SE_EFFECTSHADER_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/EffectParameter.h"
#include "Graphics/Shader/EffectTechnique.h"
#include "Graphics/Shader/EffectFunction.h"

namespace SonataEngine
{

class ShaderSystem;

/**
	@brief Shader effect.

	An effect contains several techniques.
*/
class SE_GRAPHICS_EXPORT EffectShader : public NamedObject
{
public:
	/** @name Properties. */
	//@{
	virtual ShaderSystem* GetShaderSystem() const = 0;
	//@}

	/** @name Parameters. */
	//@{
	virtual int GetParameterCount() const = 0;
	virtual EffectParameter* GetParameterByIndex(int index) const = 0;
	virtual EffectParameter* GetParameterByName(const String& name) const = 0;
	virtual EffectParameter* GetParameterBySemantic(const String& semantic) const = 0;
	//@}

	/** @name Techniques. */
	//@{
	virtual int GetTechniqueCount() const = 0;
	virtual EffectTechnique* GetTechniqueByIndex(int index) const = 0;
	virtual EffectTechnique* GetTechniqueByName(const String& name) const = 0;
	//@}

	/** @name Functions. */
	//@{
	virtual int GetFunctionCount() const = 0;
	virtual EffectFunction* GetFunctionByIndex(int index) const = 0;
	virtual EffectFunction* GetFunctionByName(const String& name) const = 0;
	//@}

	/** @name Operations. */
	//@{
	/** Starts an active technique. */
	virtual void BeginTechnique(EffectTechnique* technique) = 0;

	/** Starts an active technique, referenced by its index. */
	virtual void BeginTechniqueFromIndex(int index) = 0;

	/** Retrieve a handle for the currently active technique. */
	virtual EffectTechnique* GetActiveTechnique() = 0;

	/** Ends the active technique. */
	virtual void EndTechnique() = 0;
	//@}
};

SEPointer(EffectShader);

}

#endif

/*=============================================================================
FFPEffectShader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FFPEFFECTSHADER_H_
#define _SE_FFPEFFECTSHADER_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/EffectShader.h"
#include "Graphics/Shader/FFPTechnique.h"

namespace SonataEngine
{

/**
	@brief Fixed Function Pipeline shader effect.
*/
class SE_GRAPHICS_EXPORT FFPEffectShader : public EffectShader
{
	SE_DECLARE_CLASS(FFPEffectShader, NamedObject);

public:
	typedef Array<FFPTechnique*> TechniqueList;

protected:
	TechniqueList _techniques;
	FFPTechnique* _activeTechnique;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	FFPEffectShader();

	/** Destructor. */
	virtual ~FFPEffectShader();
	//@}

	virtual ShaderSystem* GetShaderSystem() const { return NULL; }

	virtual int GetParameterCount() const;
	virtual EffectParameter* GetParameterByIndex(int index) const;
	virtual EffectParameter* GetParameterByName(const String& name) const;
	virtual EffectParameter* GetParameterBySemantic(const String& semantic) const;

	virtual int GetTechniqueCount() const;
	virtual EffectTechnique* GetTechniqueByIndex(int index) const;
	virtual EffectTechnique* GetTechniqueByName(const String& name) const;

	virtual int GetFunctionCount() const;
	virtual EffectFunction* GetFunctionByIndex(int index) const;
	virtual EffectFunction* GetFunctionByName(const String& name) const;

	virtual void BeginTechnique(EffectTechnique* technique);
	virtual void BeginTechniqueFromIndex(int index);
	virtual EffectTechnique* GetActiveTechnique();
	virtual void EndTechnique();

	/** @name Techniques. */
	//@{
	bool AddTechnique(FFPTechnique* technique);
	bool RemoveTechnique(FFPTechnique* technique);
	void RemoveAllTechniques();
	//@}
};

SEPointer(FFPEffectShader);

}

#endif

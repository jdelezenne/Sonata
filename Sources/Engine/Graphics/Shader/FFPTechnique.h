/*=============================================================================
FFPTechnique.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FFPTECHNIQUE_H_
#define _SE_FFPTECHNIQUE_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/EffectTechnique.h"
#include "Graphics/Shader/FFPPass.h"

namespace SonataEngine
{

/**
	@brief Fixed Function Pipeline shader technique.
*/
class SE_GRAPHICS_EXPORT FFPTechnique : public EffectTechnique
{
	SE_DECLARE_CLASS(FFPTechnique, NamedObject);
	SE_BEGIN_REFLECTION(FFPTechnique);
	SE_END_REFLECTION(FFPTechnique);

public:
	typedef Array<FFPPass*> PassList;

protected:
	PassList _passes;
	FFPPass* _activePass;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	FFPTechnique();

	/** Destructor. */
	virtual ~FFPTechnique();
	//@}

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

	/** @name Passes. */
	//@{
	bool AddPass(FFPPass* pass);
	bool RemovePass(FFPPass* pass);
	void RemoveAllPasses();
	//@}
};

SEPointer(FFPTechnique);

}

#endif

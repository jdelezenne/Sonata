/*=============================================================================
EffectTechnique.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EFFECTTECHNIQUE_H_
#define _SE_EFFECTTECHNIQUE_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/EffectPass.h"

namespace SonataEngine
{

/**
	@brief Effect Shader Technique.

	A technique is a fallback mechanism to achieve an effect using different methods.
	Techniques can be used to describe an effect for different graphics hardware.
	They can also be used for functionaility, level-of-detail or performance fallbacks.
	A technique contains several passes.
*/
class SE_GRAPHICS_EXPORT EffectTechnique : public NamedObject
{
public:
	/** @name Passes. */
	//@{
	virtual int GetPassCount() const = 0;
	virtual EffectPass* GetPassByIndex(int index) const = 0;
	virtual EffectPass* GetPassByName(const String& name) const = 0;
	//@}

	/** @name Annotations. */
	//@{
	virtual int GetAnnotationCount() const = 0;
	virtual EffectAnnotation* GetAnnotationByIndex(int index) const = 0;
	virtual EffectAnnotation* GetAnnotationByName(const String& name) const = 0;
	//@}

	/** @name Operations. */
	//@{
	/** Begins a pass, within the active technique. */
	virtual void BeginPass(EffectPass* pass) = 0;

	/** Begins a pass, within the active technique, referenced by its index. */
	virtual void BeginPassFromIndex(int index) = 0;

	/** Retrieve a handle for the currently active pass. */
	virtual EffectPass* GetActivePass() = 0;

	/** Ends the active pass. */
	virtual void EndPass() = 0;
	//@}
};

SEPointer(EffectTechnique);

}

#endif

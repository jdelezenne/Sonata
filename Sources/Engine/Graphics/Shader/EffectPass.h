/*=============================================================================
EffectPass.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EFFECTPASS_H_
#define _SE_EFFECTPASS_H_

#include "Graphics/Common.h"
#include "Graphics/Shader/EffectAnnotation.h"
#include "Graphics/Shader/ShaderState.h"
#include "Graphics/Shader/ShaderProgram.h"

namespace SonataEngine
{

/**
	@brief Effect Shader Pass.

	A pass contains several states and shader programs.
*/
class SE_GRAPHICS_EXPORT EffectPass : public NamedObject
{
public:
	/** @name Properties. */
	//@{
	/** Gets the vertex program. */
	virtual ShaderProgram* GetVertexProgram() const = 0;

	/** Gets the pixel program. */
	virtual ShaderProgram* GetPixelProgram() const = 0;

	/** Gets the geometry program. */
	virtual ShaderProgram* GetGeometryProgram() const = 0;
	//@}

	/** @name Annotations. */
	//@{
	virtual int GetAnnotationCount() const = 0;
	virtual EffectAnnotation* GetAnnotationByIndex(int index) const = 0;
	virtual EffectAnnotation* GetAnnotationByName(const String& name) const = 0;
	//@}
};

SEPointer(EffectPass);

}

#endif

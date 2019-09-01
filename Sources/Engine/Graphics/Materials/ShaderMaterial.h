/*=============================================================================
ShaderMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SHADERMATERIAL_H_
#define _SE_SHADERMATERIAL_H_

#include "Graphics/Common.h"
#include "Graphics/SceneManager.h"

namespace SonataEngine
{

class MeshPart;

/**
	@brief Shader Material.

	A material represents a surface shader.
*/
class SE_GRAPHICS_EXPORT ShaderMaterial : public NamedObject
{
public:
	/** @name Constructor / Destructor. */
	//@{
	/** Constructor. */
	ShaderMaterial();

	/** Destructor. */
	virtual ~ShaderMaterial();
	//@}

	/** @name Properties. */
	//@{
	/** Gets a value indicating whether this material supports a stencil pass operation. */
	virtual bool SupportsStencilPass() const;

	/** Gets a value indicating whether this material supports a z-pass operation. */
	virtual bool SupportsZPass() const;
	//@}

	/** @name Operations. */
	//@{
	/** Initializes the material. */
	virtual void Initialize() = 0;

	/** Setups the material for the specified geometry. */
	virtual void SetupMaterial(SceneState* sceneState) = 0;

	/** Setups the material for the specified geometry. */
	virtual void SetupGeometry(MeshPart* meshPart) = 0;

	/** Setups the material for the active pass. */
	virtual void SetupPass(SceneState* sceneState, MeshPart* meshPart) = 0;

	/** Begins the material. */
 	virtual void BeginMaterial() const = 0;

	/** Ends the material. */
	virtual void EndMaterial() const = 0;

	/** Gets the number of passes. */
	virtual int GetPassCount() const = 0;

	/** Begins the pass. */
	virtual void BeginPass(int index) const = 0;

	/** Ends the active pass. */
	virtual void EndPass() const = 0;
	//@}
};

SEPointer(ShaderMaterial);

}

#endif

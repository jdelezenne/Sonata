/*=============================================================================
UberMaterial.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UBERMATERIAL_H_
#define _SE_UBERMATERIAL_H_

#include "Graphics/Common.h"
#include "Graphics/Materials/EffectMaterial.h"

namespace SonataEngine
{

/**
	@brief Uber Material.
	DirectX Standard Annotations and Semantics.
*/
class SE_GRAPHICS_EXPORT UberMaterial : public EffectMaterial
{
protected:

public:
	UberMaterial();
	virtual ~UberMaterial();

	virtual void Initialize();
	virtual void SetupMaterial(SceneState* sceneState);
	virtual void SetupGeometry(MeshPart* meshPart);
	virtual void SetupPass(SceneState* sceneState, MeshPart* meshPart);


};

SEPointer(UberMaterial);

}

#endif

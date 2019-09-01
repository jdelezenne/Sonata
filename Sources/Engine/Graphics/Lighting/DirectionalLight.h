/*=============================================================================
DirectionalLight.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DIRECTIONALLIGHT_H_
#define _SE_DIRECTIONALLIGHT_H_

#include "Core/Core.h"
#include "Graphics/Lighting/Light.h"

namespace SonataEngine
{

/**
	@brief Directional light.
*/
class SE_GRAPHICS_EXPORT DirectionalLight : public Light
{
	SE_DECLARE_CLASS(DirectionalLight, Light);

protected:
	Vector3 _direction;

public:
	/** @name Constructors / Destructor. */
	//@{
	DirectionalLight();
	virtual ~DirectionalLight();
	//@}

	/** @name Properties. */
	//@{
	virtual LightType GetLightType() const { return LightType_Directional; }
	//@}
};

SEPointer(DirectionalLight);

}

#endif

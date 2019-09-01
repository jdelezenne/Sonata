/*=============================================================================
SpotLight.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SPOTLIGHT_H_
#define _SE_SPOTLIGHT_H_

#include "Core/Core.h"
#include "Graphics/Lighting/PointLight.h"

namespace SonataEngine
{

/**
	@brief Spot light.
*/
class SE_GRAPHICS_EXPORT SpotLight : public PointLight
{
	SE_DECLARE_CLASS(SpotLight, PointLight);
	SE_BEGIN_REFLECTION(SpotLight);
		SE_Field(_innerAngle, real32, Public);
		SE_Field(_outerAngle, real32, Public);
		SE_Field(_falloffExponent, real32, Public);
	SE_END_REFLECTION(SpotLight);

protected:
	real32 _innerAngle;
	real32 _outerAngle;
	real32 _falloffExponent;
	Matrix4 _lightProjection;
	Matrix4 _lightView;
	Matrix4 _lightViewProjection;
	Frustum _lightFrustum;

public:
	/** @name Constructors / Destructor. */
	//@{
	SpotLight();
	virtual ~SpotLight();
	//@}

	/** @name Properties. */
	//@{
	virtual LightType GetLightType() const { return LightType_Spot; }

	virtual void SetRange(real32 value);

	/** Gets or sets the inner angle of the light. */
	real32 GetInnerAngle() const { return _innerAngle; }
	void SetInnerAngle(real32 value);

	/** Gets or sets the outer angle of the light. */
	real32 GetOuterAngle() const { return _outerAngle; }
	void SetOuterAngle(real32 value);

	/** Gets or sets the falloff exponent of the light. */
	real32 GetFalloffExponent() const { return _falloffExponent; }
	void SetFalloffExponent(real32 value) { _falloffExponent = value; }

	/** Gets the projection transform of the light. */
	const Matrix4& GetLightProjection() const { return _lightView; }

	/** Gets the view transform of the light. */
	const Matrix4& GetLightView() const { return _lightView; }

	/** Gets the view projection transform of the light. */
	const Matrix4& GetLightViewProjection() const { return _lightViewProjection; }

	/** Gets the view frustum of the light. */
	const Frustum& GetLightFrustum() const { return _lightFrustum; }
	//@}

protected:
	virtual void _UpdateWorldTransform();
	virtual void _UpdateLightProjection();
	virtual void _UpdateLightViewProjection();
};

SEPointer(SpotLight);

}

#endif

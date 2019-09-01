/*=============================================================================
PointLight.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_POINTLIGHT_H_
#define _SE_POINTLIGHT_H_

#include "Core/Core.h"
#include "Graphics/Lighting/Light.h"

namespace SonataEngine
{

/** Light Attenuation . */
enum LightAttenuation
{
	/// Constant Attenuation. Attenuation = 1.0.
	LightAttenuation_Constant,

	/// Linear Attenuation. Attenuation = Distance.
	LightAttenuation_Linear,

	/// Quadratic Attenuation. Attenuation = Distance * Distance.
	LightAttenuation_Quadratic,

	/// Custom Attenuation. Attenuation = Distance.
	LightAttenuation_Custom
};

SE_BEGIN_ENUM(LightAttenuation);
	SE_Enum(Constant);
	SE_Enum(Linear);
	SE_Enum(Quadratic);
	SE_Enum(Custom);
SE_END_ENUM(LightAttenuation);

/**
	@brief Point light.
	The light is initialized with a position of (0, 0, 0), a constant attenuation factor of 0.0, a linear attenuation factor of 1.0, and a quadratic attenuation factor of 0.0.
*/
class SE_GRAPHICS_EXPORT PointLight : public Light
{
	SE_DECLARE_CLASS(PointLight, Light);
	SE_BEGIN_REFLECTION(PointLight);
		SE_Field(_range, real32, Public);
		SE_Field(_attenuation, LightAttenuation, Public);
		SE_Field(_constantAttenuation, real32, Public);
		SE_Field(_linearAttenuation, real32, Public);
		SE_Field(_quadraticAttenuation, real32, Public);
	SE_END_REFLECTION(PointLight);

protected:
	real32 _range;
	LightAttenuation _attenuation;
	real32 _constantAttenuation;
	real32 _linearAttenuation;
	real32 _quadraticAttenuation;

public:
	/** @name Constructors / Destructor. */
	//@{
	PointLight();
	virtual ~PointLight();
	//@}

	/** @name Properties. */
	//@{
	virtual LightType GetLightType() const { return LightType_Point; }

	/** Get or sets the range of the light. */
	real32 GetRange() const { return _range; }
	virtual void SetRange(real32 value);

	/**
		Get or sets the attenuation type of the light.
		@remarks If LightAttenuation.Custom is set, the light equation will use
		the constant, linear and quadratic values.
	*/
	LightAttenuation GetAttenuation() const { return _attenuation; }
	void SetAttenuation(LightAttenuation value) { _attenuation = value; }

	/** Get or sets the constant attenuation of the light. */
	real32 GetConstantAttenuation() const { return _constantAttenuation; }
	void SetConstantAttenuation(real32 value) { _constantAttenuation = value; }

	/** Get or sets the linear attenuation of the light. */
	real32 GetLinearAttenuation() const { return _linearAttenuation; }
	void SetLinearAttenuation(real32 value) { _linearAttenuation = value; }

	/** Get or sets the quadratic attenuation of the light. */
	real32 GetQuadraticAttenuation() const { return _quadraticAttenuation; }
	void SetQuadraticAttenuation(real32 value) { _quadraticAttenuation = value; }
	//@}
};

SEPointer(PointLight);

}

#endif

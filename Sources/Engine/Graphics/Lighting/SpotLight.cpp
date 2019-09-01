/*=============================================================================
SpotLight.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SpotLight.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(SpotLight);
SE_IMPLEMENT_REFLECTION(SpotLight);

SpotLight::SpotLight() :
	PointLight(),
	_innerAngle(30.0f),
	_outerAngle(40.0f),
	_falloffExponent(180.0f)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetRange(real32 value)
{
	_range = value;
	_UpdateLightProjection();
}

void SpotLight::SetInnerAngle(real32 value)
{
	_innerAngle = value;
	_UpdateLightProjection();
}

void SpotLight::SetOuterAngle(real32 value)
{
	_outerAngle = value;
	_UpdateLightProjection();
}

void SpotLight::_UpdateWorldTransform()
{
	super::_UpdateWorldTransform();

	_lightView = Matrix4::Invert(_worldTransform);

	_UpdateLightViewProjection();
}

void SpotLight::_UpdateLightProjection()
{
	_lightProjection = Matrix4::CreatePerspective(_outerAngle, 1.0f, 0.1f, _range);

	_lightFrustum.SetMatrix(_lightProjection);

	_UpdateLightViewProjection();
}

void SpotLight::_UpdateLightViewProjection()
{
	_lightViewProjection = _lightView * _lightProjection;
}

}

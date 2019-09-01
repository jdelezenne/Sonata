/*=============================================================================
PointLight.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PointLight.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(PointLight);
SE_IMPLEMENT_REFLECTION(PointLight);

PointLight::PointLight() :
	Light(),
	_range(1000.0f),
	_attenuation(LightAttenuation_Custom),
	_constantAttenuation(0.0f),
	_linearAttenuation(1.0f),
	_quadraticAttenuation(0.0f)
{
}

PointLight::~PointLight()
{
}

void PointLight::SetRange(real32 value)
{
	_range = value;

	SetLocalBoundingSphere(BoundingSphere(Vector3::Zero, _range));
}

}

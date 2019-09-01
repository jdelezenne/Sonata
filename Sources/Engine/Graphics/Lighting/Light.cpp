/*=============================================================================
Light.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Light.h"

namespace SonataEngine
{

SE_IMPLEMENT_ABSTRACT(Light);
SE_IMPLEMENT_REFLECTION(Light);

Light::Light() :
	SceneObject(),
	_isEnabled(true),
	_ambientColor(Color32::Black),
	_diffuseColor(Color32::White),
	_specularColor(Color32::White),
	_intensity(1.0f),
	_isShadowCaster(true)
{
}

Light::~Light()
{
}

void Light::AddLightGroup(const String& value)
{
	if (!_lightGroups.Contains(value))
	{
		_lightGroups.Add(value);
	}
}

void Light::RemoveLightGroup(const String& value)
{
	if (_lightGroups.Contains(value))
	{
		_lightGroups.Remove(value);
	}
}

}

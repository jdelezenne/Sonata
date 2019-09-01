/*=============================================================================
Light.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Light.h"
#include "Common.h"
#include "Scene.h"

namespace Raytracer
{
	SE_IMPLEMENT_ABSTRACT(RTLight);
	SE_IMPLEMENT_REFLECTION(RTLight);

	RTLight::RTLight() :
		RefObject()
	{
		_Position = Vector3::Zero;
		_Direction = Vector3::ZAxis;
	}

	RTLight::~RTLight()
	{
	}


	SE_IMPLEMENT_CLASS(RTPointLight);
	SE_IMPLEMENT_REFLECTION(RTPointLight);

	RTPointLight::RTPointLight() :
		RTLight()
	{
		_Colour = Colour32::White;
		_Shadow = true;
		_Factor = 0.0f;
		_Attenuation = false;
		_FallStart = 0.3f;
		_FallStop = 0.7f;
	}

	RTPointLight::~RTPointLight()
	{
	}

	real32 RTPointLight::GetIntensity(RenderState& state)
	{
		// Shadow
		if (_Shadow && state._Options->_Shadows)
		{
			real32 intensity;
			state._Scene->TraceShadow(state, this, intensity);
			return intensity;
		}
		else
		{
			return 1.0f;
		}
	}


	RTSpotLight::RTSpotLight() :
		RTLight()
	{
	}

	RTSpotLight::~RTSpotLight()
	{
	}
}

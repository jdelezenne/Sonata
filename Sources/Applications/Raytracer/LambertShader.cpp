/*=============================================================================
LambertShader.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "LambertShader.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(LambertShader);
	SE_IMPLEMENT_REFLECTION(LambertShader);

	LambertShader::LambertShader()
	{
		_Ambient = Colour32(0.3f, 0.3f, 0.3f);
		_Diffuse = Colour32(0.7f, 0.7f, 0.7f);
	}

	LambertShader::~LambertShader()
	{
	}

	void LambertShader::Shade(RenderState& state, Colour32& result)
	{
		int n;
		int n_light;
		Colour32 colour;
		Colour32 sum;
		real32 dotNormalLDir;

		result.r *= _Ambient.r;
		result.g *= _Ambient.g;
		result.b *= _Ambient.b;

		n_light = _Lights.Count();

		// Loop over all light sources
		for (n=0; n < n_light; n++)
		{
			RTLight* light = _Lights[n];

			colour = light->GetColour();
			sum.r = sum.g = sum.b = 0.0f;

			if (light->GetLightType() == LightType_Direction)
			{
				Vector3 lightDir = Vector3::Normalize(light->GetDirection());
				// Dot product of the intersection normal and the light direction
				real32 dotNormalLDir = Vector3::Dot(state._Normal, lightDir);

				if (dotNormalLDir > 0.0f)
				{
					real32 intensity = light->GetIntensity(state);

					// Lambert's cosine law
					sum.r += dotNormalLDir * _Diffuse.r * colour.r * intensity;
					sum.g += dotNormalLDir * _Diffuse.g * colour.g * intensity;
					sum.b += dotNormalLDir * _Diffuse.b * colour.b * intensity;
				}
			}

			else if (light->GetLightType() == LightType_Point)
			{
				Vector3 lightDir = Vector3::Normalize(light->GetPosition() - state._Point);
				// Dot product of the intersection normal and the light direction
				dotNormalLDir = Vector3::Dot(state._Normal, lightDir);

				if (dotNormalLDir > 0.0f)
				{
					real32 intensity = 1.0f;//light->GetIntensity(state);

					// Lambert's cosine law
					sum.r += dotNormalLDir * _Diffuse.r * colour.r * intensity;
					sum.g += dotNormalLDir * _Diffuse.g * colour.g * intensity;
					sum.b += dotNormalLDir * _Diffuse.b * colour.b * intensity;
				}
			}

			result.r += sum.r;
			result.g += sum.g;
			result.b += sum.b;
		}
	}
}

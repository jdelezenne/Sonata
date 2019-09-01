/*=============================================================================
PhongShader.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "PhongShader.h"

namespace Raytracer
{
	SE_IMPLEMENT_CLASS(PhongShader);
	SE_IMPLEMENT_REFLECTION(PhongShader);

	PhongShader::PhongShader()
	{
		_Ambient = Colour32(0.3f, 0.3f, 0.3f);
		_Diffuse = Colour32(0.7f, 0.7f, 0.7f);

		_Specular = Colour32(1.0f, 1.0f, 1.0f);
		_Exponent = 50.0f;
	}

	PhongShader::~PhongShader()
	{
	}

	void PhongShader::Shade(RenderState& state, Colour32& result)
	{
		int n;
		int n_light;
		Colour32 colour;
		Colour32 sum;
		real32 dotNormalLDir;
		real32 dotVDirSpec;
		real32 spec;

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
					real32 intensity = light->GetIntensity(state);

					// Lambert's cosine law
					sum.r += dotNormalLDir * _Diffuse.r * colour.r * intensity;
					sum.g += dotNormalLDir * _Diffuse.g * colour.g * intensity;
					sum.b += dotNormalLDir * _Diffuse.b * colour.b * intensity;
				}

				// Phong's cosine power
				Vector3 specDir = lightDir - 2.0f * dotNormalLDir * state._Normal;
				dotVDirSpec = Vector3::Dot(state._Ray.Direction, specDir);
				spec = Math::Pow(dotVDirSpec, _Exponent);
				if (spec > 0.0f)
				{
					sum.r += spec * _Specular.r * colour.r;
					sum.g += spec * _Specular.g * colour.g;
					sum.b += spec * _Specular.b * colour.b;
				}
			}

			result.r += sum.r;
			result.g += sum.g;
			result.b += sum.b;
		}
	}
}

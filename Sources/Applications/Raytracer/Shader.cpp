/*=============================================================================
RTShader.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Shader.h"
#include "Scene.h"

namespace Raytracer
{
	SE_IMPLEMENT_ABSTRACT(RTShader);
	SE_IMPLEMENT_REFLECTION(RTShader);

	RTShader::RTShader()
	{
		_Transparency = Colour32::Black;
		_Reflectivity = Colour32::Black;
		_IndexOfRefraction = 1.0f;
		_Translucency = 0.0f;
		_Incandescence = Colour32::White;
	}

	RTShader::~RTShader()
	{
	}

	void RTShader::Reflect(RenderState& state, Colour32& result)
	{
		if (_Reflectivity.r > 0.0f || _Reflectivity.g > 0.0f || _Reflectivity.b > 0.0f)
		{
			if (state._ReflectionDepth < state._Options->_ReflectionDepth &&
				state._TraceDepth < state._Options->_TraceDepth)
			{
				RayType rayType = state._RayType;
				Vector3 rayOrigin = state._Ray.Origin;
				Vector3 rayDir = state._Ray.Direction;
				state._ReflectionDepth++;
				state._TraceDepth++;
				Vector3 point = state._Point;
				Vector3 normal = state._Normal;
				Vector2 texCoord = state._TexCoord;

				state._RayType = RayType_Reflect;
				// Compute the reflection direction
				Vector3 reflectDir = state._Ray.Direction - 2.0f *
					Vector3::Dot(state._Ray.Direction, state._Normal) * state._Normal;
				state._Ray.Origin = state._Point + reflectDir * Math::Epsilon;
				state._Ray.Direction = reflectDir;

				// Trace the reflection
				Colour32 colour = Colour32::Black;
				state._Scene->Raytrace(state, colour);

				state._Point = point;
				state._Normal = normal;
				state._TexCoord = texCoord;
				state._TraceDepth--;
				state._ReflectionDepth--;
				state._Ray.Direction = rayDir;
				state._Ray.Origin = rayOrigin;
				state._RayType = rayType;

				result.r = colour.r * _Reflectivity.r + result.r * (1.0f - _Reflectivity.r);
				result.g = colour.g * _Reflectivity.g + result.g * (1.0f - _Reflectivity.g);
				result.b = colour.b * _Reflectivity.b + result.b * (1.0f - _Reflectivity.b);
			}
		}
	}

	void RTShader::Refract(RenderState& state, Colour32& result)
	{
		if (_Transparency.r > 0.0f || _Transparency.g > 0.0f || _Transparency.b > 0.0f)
		{
			if (state._RefractionDepth < state._Options->_RefractionDepth &&
				state._TraceDepth < state._Options->_TraceDepth)
			{
				real32 n = state._IORIn / _IndexOfRefraction;
				Vector3 N = state._Normal * (real32)result;
				real32 cosI = -Vector3::Dot(N, state._Ray.Direction);
				real32 cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
				if (cosT2 > 0.0f)
				{
					RayType rayType = state._RayType;
					Vector3 rayOrigin = state._Ray.Origin;
					Vector3 rayDir = state._Ray.Direction;
					state._RefractionDepth++;
					state._TraceDepth++;
					Vector3 point = state._Point;
					Vector3 normal = state._Normal;
					Vector2 texCoord = state._TexCoord;
					real32 ior = state._IORIn;

					state._IORIn = _IndexOfRefraction;
					// Invert the normal if not in the view direction
					if (Vector3::Dot(state._Ray.Direction, state._Normal) > 0.0)
					{
						state._Normal = -state._Normal;
						state._IORIn = 1.0f / _IndexOfRefraction;
					}

					state._RayType = RayType_Refract;

					Vector3 refracDir = (n * state._Ray.Direction) + (n * cosI - Math::Sqrt(cosT2)) * N;
					state._Ray.Origin = state._Point + refracDir * Math::Epsilon;
					state._Ray.Direction = refracDir;

					// Trace the refraction
					Colour32 colour = Colour32::Black;
					state._Scene->Raytrace(state, colour);

					state._IORIn = ior;
					state._Point = point;
					state._Normal = normal;
					state._TexCoord = texCoord;
					state._TraceDepth--;
					state._RefractionDepth--;
					state._Ray.Direction = rayDir;
					state._Ray.Origin = rayOrigin;
					state._RayType = rayType;

					// Beer's law
					//Colour32 absorbance = _Diffuse * 0.15f * -dist;
					//Colour32 transparency = Color( expf( absorbance.r ), expf( absorbance.g ), expf( absorbance.b ) );

					result.r = colour.r * _Transparency.r;
					result.g = colour.g * _Transparency.g;
					result.b = colour.b * _Transparency.b;
				}
			}
		}
	}
}

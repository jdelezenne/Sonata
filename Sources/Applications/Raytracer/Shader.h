/*=============================================================================
RTShader.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_SHADER_H_
#define _RAYTRACER_SHADER_H_

#include "Common.h"
#include "Light.h"

namespace Raytracer
{
	class RTShader : public RefObject
	{
		SE_DECLARE_ABSTRACT(RTShader, Object);

		SE_BEGIN_REFLECTION(RTShader);
			SE_Field(_Transparency, Colour32, Public);
			SE_Field(_Reflectivity, Colour32, Public);
			SE_Field(_IndexOfRefraction, real32, Public);
			SE_Field(_Translucency, real32, Public);
			SE_Field(_Incandescence, Colour32, Public);
		SE_END_REFLECTION(RTShader);

	public:
		//@{
		RTShader();

		virtual ~RTShader();
		//@}

		//@{
		Colour32 GetTransparency() const { return _Transparency; }
		void SetTransparency(Colour32 value) { _Transparency = value; }

		Colour32 GetReflectivity() const { return _Reflectivity; }
		void SetReflectivity(Colour32 value) { _Reflectivity = value; }

		real32 GetIndexOfRefraction() const { return _IndexOfRefraction; }
		void SetIndexOfRefraction(real32 value) { _IndexOfRefraction = value; }

		real32 GetTranslucency() const { return _Translucency; }
		void SetTranslucency(real32 value) { _Translucency = value; }

		Colour32 GetIncandescence() const { return _Incandescence; }
		void SetIncandescence(Colour32 value) { _Incandescence = value; }
		//@}

		//@{
		virtual void Shade(RenderState& state, Colour32& result) = 0;

		virtual void Reflect(RenderState& state, Colour32& result);

		virtual void Refract(RenderState& state, Colour32& result);
		//@}

		List<RTLight*>& GetLights() { return _Lights; }

	protected:
		List<RTLight*> _Lights;

		Colour32 _Transparency;
		Colour32 _Reflectivity;
		real32 _IndexOfRefraction;
		real32 _Translucency;
		Colour32 _Incandescence;
	};
}

#endif 

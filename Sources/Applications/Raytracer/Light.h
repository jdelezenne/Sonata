/*=============================================================================
Light.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_LIGHT_H_
#define _RAYTRACER_LIGHT_H_

#include "Common.h"
#include "SceneObject.h"

namespace Raytracer
{
	enum LightType
	{
		LightType_Point,
		LightType_Direction,
		LightType_Spot
	};

	class RTLight : public RefObject
	{
		SE_DECLARE_ABSTRACT(RTLight, Object);

		SE_BEGIN_REFLECTION(RTLight);
			SE_Field(_Position, Vector3, Public);
			SE_Field(_Direction, Vector3, Public);
			SE_Field(_Colour, Colour32, Public);
		SE_END_REFLECTION(RTLight);

	public:
		//@{
		RTLight();
		virtual ~RTLight();
		//@}

		//@{
		virtual LightType GetLightType() const = 0;

		Vector3 GetPosition() const { return _Position; }
		void SetPosition(Vector3 value) { _Position = value; }

		Vector3 GetDirection() const { return _Direction; }
		void SetDirection(Vector3 value) { _Direction = value; }

		// colour of light source
		Colour32 GetColour() const { return _Colour; }
		void SetColour(Colour32 value) { _Colour = value; }
		//@}

		//@{
		virtual real32 GetIntensity(RenderState& state) = 0;
		//@}

	protected:
		Vector3 _Position;
		Vector3 _Direction;
		Colour32 _Colour;
	};

	class RTPointLight : public RTLight
	{
		SE_DECLARE_CLASS(RTPointLight, RTLight);

		SE_BEGIN_REFLECTION(RTPointLight);
			SE_Field(_Shadow, bool, Public);
			SE_Field(_Factor, real32, Public);
			SE_Field(_Attenuation, bool, Public);
			SE_Field(_FallStart, real32, Public);
			SE_Field(_FallStop, real32, Public);
		SE_END_REFLECTION(RTPointLight);

	public:
		//@{
		RTPointLight();
		virtual ~RTPointLight();
		//@}

		//@{
		virtual LightType GetLightType() const { return LightType_Point; }

		// light casts shadows
		bool GetShadow() const { return _Shadow; }
		void SetShadow(bool value) { _Shadow = value; }

		// makes opaque objects transparent
		real32 GetFactor() const { return _Factor; }
		void SetFactor(real32 value) { _Factor = value; }

		// distance attenuation
		bool GetAttenuation() const { return _Attenuation; }
		void SetAttenuation(bool value) { _Attenuation = value; }

		// if atten, distance range
		real32 GetFallStart() const { return _FallStart; }
		void SetFallStart(real32 value) { _FallStart = value; }

		real32 GetFallStop() const { return _FallStop; }
		void SetFallStop(real32 value) { _FallStop = value; }
		//@}

		//@{
		virtual real32 GetIntensity(RenderState& state);
		//@}

	protected:
		bool _Shadow;
		real32 _Factor;
		bool _Attenuation;
		real32 _FallStart;
		real32 _FallStop;
	};

	class RTSpotLight : public RTLight
	{
	public:
		//@{
		RTSpotLight();
		virtual ~RTSpotLight();
		//@}

		//@{
		virtual LightType GetLightType() const { return LightType_Spot; }
		//@}

	protected:
		Colour32 _Colour; // colour of light source
		bool _Shadow; // light casts shadows
		real32 _Factor; // makes opaque objects transparent
		bool _Attenuation; // distance attenuation
		real32 _Start; // if atten, distance range
		real32 _Stop;
		real32 _Cone; // inner solid cone
	};

	class RTDirectionLight
	{
	public:
		//@{
		RTDirectionLight();
		virtual ~RTDirectionLight();
		//@}

		//@{
		virtual LightType GetLightType() const { return LightType_Direction; }
		//@}

	protected:
		Colour32 _Colour; // colour of light source
		bool _Shadow; // light casts shadows
		real32 _Factor; // makes opaque objects transparent
	};
}

#endif 

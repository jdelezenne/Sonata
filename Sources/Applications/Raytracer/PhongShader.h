/*=============================================================================
PhongShader.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_PHONGSHADER_H_
#define _RAYTRACER_PHONGSHADER_H_

#include "Common.h"
#include "Shader.h"

namespace Raytracer
{
	class PhongShader : public RTShader
	{
		SE_DECLARE_CLASS(PhongShader, RTShader);

		SE_BEGIN_REFLECTION(PhongShader);
			SE_Field(_Ambient, Colour32, Public);
			SE_Field(_Diffuse, Colour32, Public);
			SE_Field(_Specular, Colour32, Public);
			SE_Field(_Exponent, real32, Public);
		SE_END_REFLECTION(PhongShader);

	public:
		//@{
		PhongShader();

		virtual ~PhongShader();
		//@}

		//@{
		// ambient colour
		Colour32 GetAmbient() const { return _Ambient; }
		void SetAmbient(Colour32 value) { _Ambient = value; }

		// diffuse colour
		Colour32 GetDiffuse() const { return _Diffuse; }
		void SetDiffuse(Colour32 value) { _Diffuse = value; }

		// specular color
		Colour32 GetSpecular() const { return _Specular; }
		void SetSpecular(Colour32 value) { _Specular = value; }

		// shinyness
		real32 GetExponent() const { return _Exponent; }
		void SetExponent(real32 value) { _Exponent = value; }
		//@}

		//@{
		virtual void Shade(RenderState& state, Colour32& result);
		//@}

	protected:
		Colour32 _Ambient;
		Colour32 _Diffuse;
		Colour32 _Specular;
		real32 _Exponent;
	};
}

#endif 

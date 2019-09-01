/*=============================================================================
LambertShader.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_LAMBERTSHADER_H_
#define _RAYTRACER_LAMBERTSHADER_H_

#include "Common.h"
#include "Shader.h"

namespace Raytracer
{
	class LambertShader : public RTShader
	{
		SE_DECLARE_CLASS(LambertShader, RTShader);

		SE_BEGIN_REFLECTION(LambertShader);
			SE_Field(_Ambient, Colour32, Public);
			SE_Field(_Diffuse, Colour32, Public);
		SE_END_REFLECTION(LambertShader);

	public:
		//@{
		LambertShader();

		virtual ~LambertShader();
		//@}

		//@{
		// ambient colour
		Colour32 GetAmbient() const { return _Ambient; }
		void SetAmbient(Colour32 value) { _Ambient = value; }

		// diffuse colour
		Colour32 GetDiffuse() const { return _Diffuse; }
		void SetDiffuse(Colour32 value) { _Diffuse = value; }
		//@}

		//@{
		virtual void Shade(RenderState& state, Colour32& result);
		//@}

	protected:
		Colour32 _Ambient;
		Colour32 _Diffuse;
	};
}

#endif 

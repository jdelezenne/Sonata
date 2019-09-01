/*=============================================================================
Gradients.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_GRADIENTS_H_
#define _PROCEDURAL_OPERATORS_GRADIENTS_H_

#include "../Common.h"

namespace Operators
{
	class GradientHOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(GradientHOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(GradientHOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(Scale, real32, Public);
			SE_Field(Offset, real32, Public);
		SE_END_REFLECTION(GradientHOperator);

	public:
		GradientHOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			Scale = 1.0f;
			Offset = 0.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 scale = Property("Scale")->GetValue().ToReal32();
			real32 offset = Property("Offset")->GetValue().ToReal32();
			RETURN_COLOURSLOT("Output", DoGradientH(a, b, scale, offset));
		}

		Color32 DoGradientH(Color32 a, Color32 b, real32 scale, real32 offset)
		{
			GET_ST();
			return mix(a, b, scale * s + offset);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 Scale;
		real32 Offset;
	};

	class GradientVOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(GradientVOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(GradientVOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(Scale, real32, Public);
			SE_Field(Offset, real32, Public);
		SE_END_REFLECTION(GradientVOperator);

	public:
		GradientVOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			Scale = 1.0f;
			Offset = 0.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 scale = Property("Scale")->GetValue().ToReal32();
			real32 offset = Property("Offset")->GetValue().ToReal32();
			RETURN_COLOURSLOT("Output", DoGradientV(a, b, scale, offset));
		}

		Color32 DoGradientV(Color32 a, Color32 b, real32 scale, real32 offset)
		{
			GET_ST();
			return mix(a, b, scale * t + offset);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 Scale;
		real32 Offset;
	};

	class RadialColorOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(RadialColorOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(RadialColorOperator);
			SE_Field(StartColor, Color8, Public);
			SE_Field(EndColor, Color8, Public);
			SE_Field(SScale, real32, Public);
			SE_Field(SOffset, real32, Public);
			SE_Field(TScale, real32, Public);
			SE_Field(TOffset, real32, Public);
		SE_END_REFLECTION(RadialColorOperator);

	public:
		RadialColorOperator() : super()
		{
			SScale = 1.0f;
			SOffset = -0.5f;
			TScale = 1.0f;
			TOffset = -0.5f;
			StartColor = Color8::Gold;
			EndColor = Color8::Blue;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			real32 SScale = Property("SScale")->GetValue().ToReal32();
			real32 SOffset = Property("SOffset")->GetValue().ToReal32();
			real32 TScale = Property("TScale")->GetValue().ToReal32();
			real32 TOffset = Property("TOffset")->GetValue().ToReal32();
			Color32 StartColor = VariantToColor32(Property("StartColor")->GetValue());
			Color32 EndColor = VariantToColor32(Property("EndColor")->GetValue());

			RETURN_COLOURSLOT("Output", Do(SScale, SOffset, TScale, TOffset, StartColor, EndColor));
		}

		Color32 Do(real32 SScale, real32 SOffset, real32 TScale, real32 TOffset, Color32 StartColor, Color32 EndColor)
		{
			GET_ST();
			return mix(StartColor, EndColor, radius(((SScale*s)+SOffset),((TScale*t)+TOffset)));
		}

	protected:
		real32 SScale;
		real32 SOffset;
		real32 TScale;
		real32 TOffset;
		Color8 StartColor;
		Color8 EndColor;
	};

	class GlowColorOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(GlowColorOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(GlowColorOperator);
			SE_Field(View, Vector3, Public);
			SE_Field(Normal, Vector3, Public);
			SE_Field(Attenuation, real32, Public);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(GlowColorOperator);

	public:
		GlowColorOperator() : super()
		{
			View = Vector3::UnitY;
			Normal = Vector3::UnitY;
			Attenuation = 2.0f;
			Color = Color8::White;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Vector3 View = VariantToVector3(Property("View")->GetValue());
			Vector3 Normal = VariantToVector3(Property("Normal")->GetValue());
			real32 Attenuation = Property("Attenuation")->GetValue().ToReal32();
			Color32 Color = VariantToColor32(Property("Color")->GetValue());

			RETURN_COLOURSLOT("Output", Do(View, Normal, Attenuation, Color));
		}

		Color32 Do(Vector3 View, Vector3 Normal, real32 Attenuation, Color32 Color)
		{
			real32 falloff = Vector3::Dot(Vector3::Normalize(Normal), Vector3::Normalize(View));
			falloff = (falloff < 0 ) ? Math::Pow(falloff, Attenuation) : 0;
			return Color * falloff;
		}

	protected:
		Vector3 View;
		Vector3 Normal;
		real32 Attenuation;
		Color8 Color;
	};

	SE_IMPLEMENT_CLASS(GradientHOperator);
	SE_IMPLEMENT_REFLECTION(GradientHOperator);

	SE_IMPLEMENT_CLASS(GradientVOperator);
	SE_IMPLEMENT_REFLECTION(GradientVOperator);

	SE_IMPLEMENT_CLASS(RadialColorOperator);
	SE_IMPLEMENT_REFLECTION(RadialColorOperator);

	SE_IMPLEMENT_CLASS(GlowColorOperator);
	SE_IMPLEMENT_REFLECTION(GlowColorOperator);
}

#endif 

/*=============================================================================
Filters.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_FILTERS_H_
#define _PROCEDURAL_OPERATORS_FILTERS_H_

#include "../Common.h"

namespace Operators
{
	class BlurOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(BlurOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(BlurOperator);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(BlurOperator);

	public:
		BlurOperator() : super()
		{
			Color = Color8::Black;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			if (!Property("Color")->IsLinked())
				Slot("Output")->SetValue(ToVariant(Color));
			else
				Slot("Output")->SetValue(ToVariant(Do()));
		}

		Color32 Do()
		{
			GET_ST();
			GET_STEP();

			Color32 Color = Color32::Clamp(
				(GetColorAt(s-ss, t-tt) +
				GetColorAt(s-ss, t   ) +
				GetColorAt(s-ss, t+tt) +
				GetColorAt(s,    t-tt) +
				GetColorAt(s,    t   ) +
				GetColorAt(s,    t+tt) +
				GetColorAt(s+ss, t-tt) +
				GetColorAt(s+ss, t   ) +
				GetColorAt(s+ss, t+tt)) / 9.0f,
				0.0f, 1.0f);

			SET_ST(s, t);
			return Color;
		}

	protected:
		Color8 Color;
	};

	class SwirlOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(SwirlOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(SwirlOperator);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(SwirlOperator);

	public:
		SwirlOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			if (!Property("Color")->IsLinked())
				Slot("Output")->SetValue(ToVariant(Color));
			else
				Slot("Output")->SetValue(ToVariant(Do()));
		}

		Color32 Do()
		{
			GET_ST();

			real32 dz = -0.2f;

			real32 x = (s - 0.5f);
			real32 y = (t - 0.5f);

			real32 dist = Math::Sqrt(x * x + y * y);
			real32 angle = Math::Atan2(y, x);
			if (angle < 0.0f) 
			{
				angle += Math::TwoPi;
			}

			Color32 Color = Color32::Clamp(GetColorAt(
				(0.5f + dist * Math::Cos(angle + dist * dz)),
				(0.5f + dist * Math::Sin(angle + dist * dz))),
				0.0f, 1.0f);

			SET_ST(s, t);
			return Color;
		}

	protected:
		Color8 Color;
	};

	class TwirlOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(TwirlOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(TwirlOperator);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(TwirlOperator);

	public:
		TwirlOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			if (!Property("Color")->IsLinked())
				Slot("Output")->SetValue(ToVariant(Color));
			else
				Slot("Output")->SetValue(ToVariant(Do()));
		}

		Color32 Do()
		{
			GET_ST();

			real32 a = Math::Atan2(-1.0f, 0.5f - 1.0f);
			if (a < 0.0)
				a += Math::TwoPi;
			real32 dx = 1.0f / a;
			real32 d  = Math::Sqrt(0.5f);
			real32 dy = 1.0f / d;

			real32 x = (s - 0.5f);
			real32 y = (t - 0.5f);

			real32 dist = Math::Sqrt(x * x + y * y);
			real32 angle = Math::Atan2(y, x);
			if (angle < 0.0f) 
			{
				angle += Math::TwoPi;
			}

			Color32 Color = Color32::Clamp(GetColorAt(
				(1.0f - dx * angle), (dy * dist)),
				0.0f, 1.0f);

			SET_ST(s, t);
			return Color;
		}

	protected:
		Color8 Color;
	};

	SE_IMPLEMENT_CLASS(BlurOperator);
	SE_IMPLEMENT_REFLECTION(BlurOperator);

	SE_IMPLEMENT_CLASS(SwirlOperator);
	SE_IMPLEMENT_REFLECTION(SwirlOperator);

	SE_IMPLEMENT_CLASS(TwirlOperator);
	SE_IMPLEMENT_REFLECTION(TwirlOperator);
}

#endif 

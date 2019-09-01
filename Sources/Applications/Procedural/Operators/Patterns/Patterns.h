/*=============================================================================
Patterns.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_PATTERNS_H_
#define _PROCEDURAL_OPERATORS_PATTERNS_H_

#include "../Common.h"

namespace Operators
{
	class CheckerOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(CheckerOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(CheckerOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(FQs, real32, Public);
			SE_Field(FQt, real32, Public);
		SE_END_REFLECTION(CheckerOperator);

	public:
		CheckerOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			FQs = 2.0f;
			FQt = 2.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 FQs = Property("FQs")->GetValue().ToReal32();
			real32 FQt = Property("FQt")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(DoChecker(a, b, FQs, FQt)));
		}

		Color32 DoChecker(Color32 a, Color32 b, real32 FQs, real32 FQt)
		{
			GET_ST();
			Color32 Ci;

			real32 smod = Math::Mod(s*FQs, 1.0f);
			real32 tmod = Math::Mod(t*FQt, 1.0f);

			if (smod < 0.5f)
			{
				if (tmod < 0.5f) 
					Ci = a;
				else
					Ci = b;
			}
			else
			{
				if (tmod < 0.5f)
					Ci = b;
				else
					Ci = a;
			}

			return Ci;
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 FQs;
		real32 FQt;
	};

	class DotsOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(DotsOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(DotsOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(FQs, real32, Public);
			SE_Field(FQt, real32, Public);
		SE_END_REFLECTION(DotsOperator);

	public:
		DotsOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			FQs = 3.0f;
			FQt = 3.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 FQs = Property("FQs")->GetValue().ToReal32();
			real32 FQt = Property("FQt")->GetValue().ToReal32();
			real32 o = DoDots(FQs, FQt);
			Slot("Output")->SetValue(ToVariant(mix(a, b, o)));
		}

		real32 DoDots(real32 FQs, real32 FQt)
		{
			GET_ST();

			real32 x = Math::Cos(Math::Pi*FQs*(t-s))*Math::Sin(Math::Pi*FQt*(s+t));
			return x*x;
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 FQs;
		real32 FQt;
	};

	class RandomColorOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(RandomColorOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(RandomColorOperator);
		SE_END_REFLECTION(RandomColorOperator);

	public:
		RandomColorOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 c(
				Math::Random(0.0f, 1.0f),
				Math::Random(0.0f, 1.0f),
				Math::Random(0.0f, 1.0f),
				Math::Random(0.0f, 1.0f));
			Slot("Output")->SetValue(ToVariant(c));
		}

	protected:
	};

	class StripesOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(StripesOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(StripesOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(Frequency, real32, Public);
			SE_Field(Direction, real32, Public);
		SE_END_REFLECTION(StripesOperator);

	public:
		StripesOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			Frequency = 6.0f;
			Direction = 0.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			GET_ST();
			Direction = s;

			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 Frequency = Property("Frequency")->GetValue().ToReal32();
			real32 Direction = Property("Direction")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(mix(a, b, Do(Frequency, Direction))));
		}

		real32 Do(real32 Frequency, real32 Direction)
		{
			return (1.0f - ((1.0f + Math::Sin(Math::Pi * Frequency * Direction)) / 2.0f));
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 Frequency;
		real32 Direction;
	};

	class SawtoothWaveOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(SawtoothWaveOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(SawtoothWaveOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(Count, real32, Public);
			SE_Field(Height, real32, Public);
		SE_END_REFLECTION(SawtoothWaveOperator);

	public:
		SawtoothWaveOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			Count = 6.0f;
			Height = 1.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 Count = Property("Count")->GetValue().ToReal32();
			real32 Height = Property("Height")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(mix(a, b, Do(Count, Height))));
		}

		real32 Do(real32 Count, real32 Height)
		{
			GET_ST();
			return (Height * Math::Mod(s*Count, 1.0f));
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 Count;
		real32 Height;
	};

	class SquareWaveOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(SquareWaveOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(SquareWaveOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(Frequency, real32, Public);
			SE_Field(Height, real32, Public);
			SE_Field(Width, real32, Public);
		SE_END_REFLECTION(SquareWaveOperator);

	public:
		SquareWaveOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			Frequency = 6.0f;
			Height = 1.0f;
			Width = 1.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 Frequency = Property("Frequency")->GetValue().ToReal32();
			real32 Height = Property("Height")->GetValue().ToReal32();
			real32 Width = Property("Width")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(mix(a, b, Do(Frequency, Height, Width))));
		}

		real32 Do(real32 Frequency, real32 Height, real32 Width)
		{
			GET_ST();
			return ((Math::Mod((Frequency*s), 1.0f) > Width) ? 0.0f : Height);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 Frequency;
		real32 Height;
		real32 Width;
	};

	class TriangleWaveOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(TriangleWaveOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(TriangleWaveOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(Count, real32, Public);
			SE_Field(Height, real32, Public);
		SE_END_REFLECTION(TriangleWaveOperator);

	public:
		TriangleWaveOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			Count = 6.0f;
			Height = 1.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 Count = Property("Count")->GetValue().ToReal32();
			real32 Height = Property("Height")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(mix(a, b, Do(Count, Height))));
		}

		real32 Do(real32 Count, real32 Height)
		{
			GET_ST();
			return (Height * (1.0f - Math::Abs(2.0f * Math::Mod(s*Count, 1.0f) - 1.0f)));
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 Count;
		real32 Height;
	};

	SE_IMPLEMENT_CLASS(CheckerOperator);
	SE_IMPLEMENT_REFLECTION(CheckerOperator);

	SE_IMPLEMENT_CLASS(DotsOperator);
	SE_IMPLEMENT_REFLECTION(DotsOperator);

	SE_IMPLEMENT_CLASS(RandomColorOperator);
	SE_IMPLEMENT_REFLECTION(RandomColorOperator);

	SE_IMPLEMENT_CLASS(StripesOperator);
	SE_IMPLEMENT_REFLECTION(StripesOperator);

	SE_IMPLEMENT_CLASS(SawtoothWaveOperator);
	SE_IMPLEMENT_REFLECTION(SawtoothWaveOperator);

	SE_IMPLEMENT_CLASS(SquareWaveOperator);
	SE_IMPLEMENT_REFLECTION(SquareWaveOperator);

	SE_IMPLEMENT_CLASS(TriangleWaveOperator);
	SE_IMPLEMENT_REFLECTION(TriangleWaveOperator);
}

#endif 

/*=============================================================================
Color.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_COLOUR_H_
#define _PROCEDURAL_OPERATORS_COLOUR_H_

#include "../Common.h"

namespace Operators
{
	class ColorAddOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorAddOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorAddOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
		SE_END_REFLECTION(ColorAddOperator);

	public:
		ColorAddOperator() : super()
		{
			ColorA = Color8::Gray;
			ColorB = Color8::Gray;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			RETURN_COLOURSLOT("Output", Do(a, b));
		}

		Color32 Do(Color32 a, Color32 b)
		{
			return (a + b);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
	};

	class ColorContrastOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorContrastOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorContrastOperator);
			SE_Field(Color, Color8, Public);
			SE_Field(Shift, real32, Public);
			SE_Field(Gain, real32, Public);
		SE_END_REFLECTION(ColorContrastOperator);

	public:
		ColorContrastOperator() : super()
		{
			Color = Color8::White;
			Shift = 0.0f;
			Gain = 1.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 Color = VariantToColor32(Property("Color")->GetValue());
			real32 Shift = Property("Shift")->GetValue().ToReal32();
			real32 Gain = Property("Gain")->GetValue().ToReal32();
			RETURN_COLOURSLOT("Output", Do(Color, Shift, Gain));
		}

		Color32 Do(Color32 Color, real32 Shift, real32 Gain)
		{
			return Color32(
				(Color.r+Shift)*Gain,
				(Color.g+Shift)*Gain,
				(Color.b+Shift)*Gain);
		}

	protected:
		Color8 Color;
		real32 Shift;
		real32 Gain;
	};

	class ColorDivOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorDivOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorDivOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
		SE_END_REFLECTION(ColorDivOperator);

	public:
		ColorDivOperator() : super()
		{
			ColorA = Color8::Gray;
			ColorB = Color8::Gray;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			RETURN_COLOURSLOT("Output", Do(a, b));
		}

		Color32 Do(Color32 a, Color32 b)
		{
			return Color32(b.r != 0 ? a.r / b.r : 0,
				b.g != 0 ? a.g / b.g : 0,
				b.b != 0 ? a.b / b.b : 0);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
	};

	class ColorIntensityOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ColorIntensityOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ColorIntensityOperator);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(ColorIntensityOperator);

	public:
		ColorIntensityOperator() : super()
		{
			Color = Color8::White;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			Color32 color = VariantToColor32(Property("Color")->GetValue());
			Slot("Output")->SetValue(Do(color));
		}

		real32 Do(Color32 color)
		{
			return (color.r + color.g + color.b) / 3.0f;
		}

	protected:
		Color8 Color;
	};

	class ColorInvertOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorInvertOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorInvertOperator);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(ColorInvertOperator);

	public:
		ColorInvertOperator() : super()
		{
			Color = Color8::White;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 color = VariantToColor32(Property("Color")->GetValue());
			Slot("Output")->SetValue(ToVariant(Do(color)));
		}

		Color32 Do(Color32 color)
		{
			return (1.0f - color);
		}

	protected:
		Color8 Color;
	};

	class ColorMixOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorMixOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorMixOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
			SE_Field(X, real32, Public);
		SE_END_REFLECTION(ColorMixOperator);

	public:
		ColorMixOperator() : super()
		{
			ColorA = Color8::Black;
			ColorB = Color8::White;
			X = 0.5f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			real32 x = Property("X")->GetValue().ToReal32();
			RETURN_COLOURSLOT("Output", Do(a, b, x));
		}

		Color32 Do(Color32 a, Color32 b, real32 x)
		{
			return mix(a, b, x);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
		real32 X;
	};

	class ColorMulOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorMulOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorMulOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
		SE_END_REFLECTION(ColorMulOperator);

	public:
		ColorMulOperator() : super()
		{
			ColorA = Color8::Gray;
			ColorB = Color8::Gray;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			RETURN_COLOURSLOT("Output", Do(a, b));
		}

		Color32 Do(Color32 a, Color32 b)
		{
			return (a * b);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
	};

	class ColorNegateOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorNegateOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorNegateOperator);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(ColorNegateOperator);

	public:
		ColorNegateOperator() : super()
		{
			Color = Color8::White;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 color = VariantToColor32(Property("Color")->GetValue());
			Slot("Output")->SetValue(ToVariant(Do(color)));
		}

		Color32 Do(Color32 color)
		{
			return -color;
		}

	protected:
		Color8 Color;
	};

	class ColorSubOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorSubOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorSubOperator);
			SE_Field(ColorA, Color8, Public);
			SE_Field(ColorB, Color8, Public);
		SE_END_REFLECTION(ColorSubOperator);

	public:
		ColorSubOperator() : super()
		{
			ColorA = Color8::White;
			ColorB = Color8::Black;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Color32 a = VariantToColor32(Property("ColorA")->GetValue());
			Color32 b = VariantToColor32(Property("ColorB")->GetValue());
			RETURN_COLOURSLOT("Output", Do(a, b));
		}

		Color32 Do(Color32 a, Color32 b)
		{
			return (a - b);
		}

	protected:
		Color8 ColorA;
		Color8 ColorB;
	};

	class ColorValueOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ColorValueOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ColorValueOperator);
			SE_Field(Value, Color8, Public);
		SE_END_REFLECTION(ColorValueOperator);

	public:
		ColorValueOperator() : super()
		{
			Value = Color8::Black;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Slot("Output")->SetValue(Property("Value")->GetValue());
		}

	protected:
		Color8 Value;
	};

	SE_IMPLEMENT_CLASS(ColorAddOperator);
	SE_IMPLEMENT_REFLECTION(ColorAddOperator);

	SE_IMPLEMENT_CLASS(ColorContrastOperator);
	SE_IMPLEMENT_REFLECTION(ColorContrastOperator);

	SE_IMPLEMENT_CLASS(ColorDivOperator);
	SE_IMPLEMENT_REFLECTION(ColorDivOperator);

	SE_IMPLEMENT_CLASS(ColorIntensityOperator);
	SE_IMPLEMENT_REFLECTION(ColorIntensityOperator);

	SE_IMPLEMENT_CLASS(ColorInvertOperator);
	SE_IMPLEMENT_REFLECTION(ColorInvertOperator);

	SE_IMPLEMENT_CLASS(ColorMixOperator);
	SE_IMPLEMENT_REFLECTION(ColorMixOperator);

	SE_IMPLEMENT_CLASS(ColorMulOperator);
	SE_IMPLEMENT_REFLECTION(ColorMulOperator);

	SE_IMPLEMENT_CLASS(ColorNegateOperator);
	SE_IMPLEMENT_REFLECTION(ColorNegateOperator);

	SE_IMPLEMENT_CLASS(ColorSubOperator);
	SE_IMPLEMENT_REFLECTION(ColorSubOperator);

	SE_IMPLEMENT_CLASS(ColorValueOperator);
	SE_IMPLEMENT_REFLECTION(ColorValueOperator);
}

#endif 

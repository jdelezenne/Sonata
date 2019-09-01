/*=============================================================================
Colour.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_COLOUR_H_
#define _PROCEDURAL_OPERATORS_COLOUR_H_

#include "../Workflow.h"
#include "../Utils.h"

#define DECLARE_COMPONENTPROPERTY(name, type, canlink, value) \
	{ \
		OperatorProperty* property = new OperatorProperty(this); \
		property->SetName(#name); \
		property->SetType(typeof(type)); \
		property->SetCanLink(false); \
		property->SetValue(ToVariant(value)); \
		_Properties.Add(property); \
	}

#define DECLARE_COMPONENTSLOT(name, type) \
	{ \
		OperatorSlot* slot = new OperatorSlot(this); \
		slot->SetName(#name); \
		slot->SetType(typeof(type)); \
		_Slots.Add(slot); \
	}

namespace Operators
{
	Colour8 ToColour8(const Colour32& c);
	Colour32 ToColour32(const Colour8& c);

	const real32 MinFilterWidth = 1.0e-6;

	real32 step(real32 edge, real32 x);

	real32 smoothstep(real32 edge0, real32 edge1, real32 x);

	real32 pulse(real32 edge0, real32 edge1, real32 x);

	real32 pulsetrain (real32 edge, real32 period, real32 x);

	real32 smoothpulse(real32 e0, real32 e1, real32 e2, real32 e3, real32 x);

	real32 smoothpulsetrain(real32 e0, real32 e1, real32 e2, real32 e3, real32 period, real32 x);

	real32 bias(real32 x, real32 b);

	real32 biasperlin(real32 x, real32 b, real32 min, real32 max);

	real32 gain(real32 x, real32 g);

	//real32 square(real32 dir, real32 high, real32 frequency, real32 offset, real32 cycle, real32 width, real32 filterkernel);

	real32 triangle(real32 dir, real32 high, real32 frequency, real32 offset);

	Colour32 mix(const Colour32& c0, const Colour32& c1, real32 f);

	Colour8 mix(const Colour8& c0, const Colour8& c1, real32 f);

	real32 blendwidth(real32 x, real32 y);

	real32 filterwidth(real32 x);

	class ImageOperator : public Operator
	{
		SE_DECLARE_CLASS(ImageOperator, Operator);

		SE_BEGIN_REFLECTION(ImageOperator);
			SE_Field(FileName, String, Public);
		SE_END_REFLECTION(ImageOperator);

	public:
		ImageOperator() : Operator()
		{
			_SourceImage = NULL;
		}

		virtual String GetOperatorName() const { return "Image"; }

		virtual void Refresh()
		{
			SE_SAFE_DELETE(_SourceImage);
			if (!FileName.IsEmpty())
			{
				_SourceImage = CreateImage(FileName);
			}

			super::Refresh();
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			real32 s, t;
			_Workflow->GetST(s, t);

			if (FileName.IsEmpty() || _SourceImage == NULL)
			{
				Slot("Output")->SetValue(ToVariant(Colour8::Black));
			}
			else
			{
				Colour8 colour = _SourceImage->GetRGB(
					s * _SourceImage->GetWidth(),
					t * _SourceImage->GetHeight());

				Slot("Output")->SetValue(ToVariant(colour));
			}
		}

	protected:
		String FileName;
		Image* _SourceImage;
	};

	class ColourValueOperator : public Operator
	{
		SE_DECLARE_CLASS(ColourValueOperator, Operator);

		SE_BEGIN_REFLECTION(ColourValueOperator);
			SE_Field(Value, Colour8, Public);
		SE_END_REFLECTION(ColourValueOperator);

	public:
		ColourValueOperator() : Operator()
		{
			Value = Colour8::Black;
		}

		virtual String GetOperatorName() const { return "Colour Value"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Slot("Output")->SetValue(Property("Value")->GetValue());
		}

	protected:
		Colour8 Value;
	};

	class RandomOperator : public Operator
	{
		SE_DECLARE_CLASS(RandomOperator, Operator);

		SE_BEGIN_REFLECTION(RandomOperator);
		SE_END_REFLECTION(RandomOperator);

	public:
		RandomOperator() : Operator()
		{
		}

		virtual String GetOperatorName() const { return "Random"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour32 c(
				Math::Random(0.0f, 1.0f),
				Math::Random(0.0f, 1.0f),
				Math::Random(0.0f, 1.0f),
				Math::Random(0.0f, 1.0f));
			Slot("Output")->SetValue(ToVariant(c));
		}

	protected:
	};

	class GradientHOperator : public Operator
	{
		SE_DECLARE_CLASS(GradientHOperator, Operator);

		SE_BEGIN_REFLECTION(GradientHOperator);
			SE_Field(ColourA, Colour8, Public);
			SE_Field(ColourB, Colour8, Public);
			SE_Field(Scale, real32, Public);
			SE_Field(Offset, real32, Public);
		SE_END_REFLECTION(GradientHOperator);

	public:
		GradientHOperator() : Operator()
		{
			ColourA = Colour8::Black;
			ColourB = Colour8::White;
			Scale = 1.0f;
			Offset = 0.0f;
		}

		virtual String GetOperatorName() const { return "Gradient H"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour32 a = VariantToColour32(Property("ColourA")->GetValue());
			Colour32 b = VariantToColour32(Property("ColourB")->GetValue());
			real32 scale = Property("Scale")->GetValue().ToReal32();
			real32 offset = Property("Offset")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(DoGradientH(a, b, scale, offset)));
		}

		Colour32 DoGradientH(Colour32 a, Colour32 b, real32 scale, real32 offset)
		{
			real32 s, t;
			_Workflow->GetST(s, t);

			return mix(a, b, scale * s + offset);
		}

	protected:
		Colour8 ColourA;
		Colour8 ColourB;
		real32 Scale;
		real32 Offset;
	};

	class GradientVOperator : public Operator
	{
		SE_DECLARE_CLASS(GradientVOperator, Operator);

		SE_BEGIN_REFLECTION(GradientVOperator);
			SE_Field(ColourA, Colour8, Public);
			SE_Field(ColourB, Colour8, Public);
			SE_Field(Scale, real32, Public);
			SE_Field(Offset, real32, Public);
		SE_END_REFLECTION(GradientVOperator);

	public:
		GradientVOperator() : Operator()
		{
			ColourA = Colour8::Black;
			ColourB = Colour8::White;
			Scale = 1.0f;
			Offset = 0.0f;
		}

		virtual String GetOperatorName() const { return "Gradient V"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour32 a = VariantToColour32(Property("ColourA")->GetValue());
			Colour32 b = VariantToColour32(Property("ColourB")->GetValue());
			real32 scale = Property("Scale")->GetValue().ToReal32();
			real32 offset = Property("Offset")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(DoGradientV(a, b, scale, offset)));
		}

		Colour32 DoGradientV(Colour32 a, Colour32 b, real32 scale, real32 offset)
		{
			real32 s, t;
			_Workflow->GetST(s, t);

			return mix(a, b, scale * t + offset);
		}

	protected:
		Colour8 ColourA;
		Colour8 ColourB;
		real32 Scale;
		real32 Offset;
	};

	class ColourAddOperator : public Operator
	{
		SE_DECLARE_CLASS(ColourAddOperator, Operator);

		SE_BEGIN_REFLECTION(ColourAddOperator);
			SE_Field(ColourA, Colour8, Public);
			SE_Field(ColourB, Colour8, Public);
		SE_END_REFLECTION(ColourAddOperator);

	public:
		ColourAddOperator() : Operator()
		{
			ColourA = Colour8::Black;
			ColourB = Colour8::White;
		}

		virtual String GetOperatorName() const { return "Colour Add"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour32 a = VariantToColour32(Property("ColourA")->GetValue());
			Colour32 b = VariantToColour32(Property("ColourB")->GetValue());
			Slot("Output")->SetValue(ToVariant(Colour32::Clamp(DoAdd(a, b), 0.0f, 1.0f)));
		}

		Colour32 DoAdd(Colour32 a, Colour32 b)
		{
			return (a + b);
		}

	protected:
		Colour8 ColourA;
		Colour8 ColourB;
	};

	class ColourMixOperator : public Operator
	{
		SE_DECLARE_CLASS(ColourMixOperator, Operator);

		SE_BEGIN_REFLECTION(ColourMixOperator);
			SE_Field(ColourA, Colour8, Public);
			SE_Field(ColourB, Colour8, Public);
			SE_Field(X, real32, Public);
		SE_END_REFLECTION(ColourMixOperator);

	public:
		ColourMixOperator() : Operator()
		{
			ColourA = Colour8::Black;
			ColourB = Colour8::White;
			X = 0.5f;
		}

		virtual String GetOperatorName() const { return "Colour Mix"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour32 a = VariantToColour32(Property("ColourA")->GetValue());
			Colour32 b = VariantToColour32(Property("ColourB")->GetValue());
			real32 x = Property("X")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(DoMix(a, b, x)));
		}

		Colour32 DoMix(Colour32 a, Colour32 b, real32 f)
		{
			return mix(a, b, f);
		}

	protected:
		Colour8 ColourA;
		Colour8 ColourB;
		real32 X;
	};

	class CheckerOperator : public Operator
	{
		SE_DECLARE_CLASS(CheckerOperator, Operator);

		SE_BEGIN_REFLECTION(CheckerOperator);
			SE_Field(ColourA, Colour8, Public);
			SE_Field(ColourB, Colour8, Public);
			SE_Field(FQs, real32, Public);
			SE_Field(FQt, real32, Public);
		SE_END_REFLECTION(CheckerOperator);

	public:
		CheckerOperator() : Operator()
		{
			ColourA = Colour8::Black;
			ColourB = Colour8::White;
			FQs = 2.0f;
			FQt = 2.0f;
		}

		virtual String GetOperatorName() const { return "Checker"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour32 a = VariantToColour32(Property("ColourA")->GetValue());
			Colour32 b = VariantToColour32(Property("ColourB")->GetValue());
			real32 FQs = Property("FQs")->GetValue().ToReal32();
			real32 FQt = Property("FQt")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(DoChecker(a, b, FQs, FQt)));
		}

		Colour32 DoChecker(Colour32 a, Colour32 b, real32 FQs, real32 FQt)
		{
			Colour32 Ci;
			real32 s, t;
			_Workflow->GetST(s, t);

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
		Colour8 ColourA;
		Colour8 ColourB;
		real32 FQs;
		real32 FQt;
	};

	class DotsOperator : public Operator
	{
		SE_DECLARE_CLASS(DotsOperator, Operator);

		SE_BEGIN_REFLECTION(DotsOperator);
			SE_Field(ColourA, Colour8, Public);
			SE_Field(ColourB, Colour8, Public);
			SE_Field(FQs, real32, Public);
			SE_Field(FQt, real32, Public);
		SE_END_REFLECTION(DotsOperator);

	public:
		DotsOperator() : Operator()
		{
			ColourA = Colour8::Black;
			ColourB = Colour8::White;
			FQs = 3.0f;
			FQt = 3.0f;
		}

		virtual String GetOperatorName() const { return "Dots"; }

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour32 a = VariantToColour32(Property("ColourA")->GetValue());
			Colour32 b = VariantToColour32(Property("ColourB")->GetValue());
			real32 FQs = Property("FQs")->GetValue().ToReal32();
			real32 FQt = Property("FQt")->GetValue().ToReal32();
			real32 o = DoDots(FQs, FQt);
			Slot("Output")->SetValue(ToVariant(mix(a, b, o)));
		}

		real32 DoDots(real32 FQs, real32 FQt)
		{
			real32 s, t;
			_Workflow->GetST(s, t);

			real32 x = Math::Cos(Math::PI*FQs*(t-s))*Math::Sin(Math::PI*FQt*(s+t));
			return x*x;
		}

	protected:
		Colour8 ColourA;
		Colour8 ColourB;
		real32 FQs;
		real32 FQt;
	};

	/*class Bricks : public Operator
	{
	public:
		Bricks() : Operator()
		{
		}

		virtual void Create()
		{
			super::Create();

			OperatorProperty* a = new OperatorProperty(this);
			a->SetName("a");
			a->SetType(typeof(Colour8));
			a->SetValue(ToVariant(Colour8::Black));
			_Properties.Add(a);

			OperatorProperty* b = new OperatorProperty(this);
			b->SetName("b");
			b->SetType(typeof(Colour8));
			b->SetValue(ToVariant(Colour8::White));
			_Properties.Add(b);

			DECLARE_COMPONENTSLOT(Output, Colour8);
		}

		virtual void Update()
		{
			Colour8 a = VariantToColour32(Property("a")->GetValue());
			Colour8 b = VariantToColour32(Property("b")->GetValue());
			real32 o = DoBricks(0.5f, 0.2f, 0.08f);
			Slot("Output")->SetValue(ToVariant(mix(a, b, o)));
		}

		real32 DoBricks(real32 BW, real32 BH, real32 MW)
		{
			real32 s, t;
			_Workflow->GetST(s, t);

			real32 W = BW + MW;
			real32 H = BH + MW;
			real32 SS = s / W;
			real32 TT = t / H;
			if (Math::Mod(TT*0.5f, 1.0f) > 0.5f)
				SS += 0.5f;

			real32 sfuzz = filterwidth(SS);
			real32 tfuzz = filterwidth(TT);
			real32 smortar = MW * 0.5f / W;
			real32 tmortar = MW * 0.5f / H;
			real32 width = (blendwidth(SS+sfuzz, smortar) - blendwidth(SS, smortar)) / sfuzz;
			real32 height = (blendwidth(TT+tfuzz, tmortar) - blendwidth(TT, tmortar)) / tfuzz;
			return smoothstep(0.0f, 1.0f, width*height);
		}
	};
*/
	/*class FBM : public Operator
	{
	public:
		FBM() : Operator()
		{
		}

		virtual void Create()
		{
			super::Create();

			OperatorProperty* Output = new OperatorProperty(this);
			Output->SetName("Output");
			Output->SetType(typeof(Colour8));
			_Properties.Add(Output);
		}

		virtual void Update()
		{
			//Slot("Output")->SetValue(ToVariant(DoFBM(4, 2, 0.5)));
		}

		real32 DoFBM(real32 octaves, real32 lacunarity, real32 gain)
		{
			real32 s, t;
			_Workflow->GetST(s, t);

			real32 amp = 1;
			Vector2 pp(s, t);
			real32 sum = 0;
			real32 fw = filterwidthp(P);
			real32 i;

			for (i = 0;  i < octaves;  i += 1)
			{
				sum += amp * filteredsnoise (pp, fw);
				amp *= gain; pp *= lacunarity;  fw *= lacunarity;
			}

			return sum;
		}
	};*/
}

#endif 

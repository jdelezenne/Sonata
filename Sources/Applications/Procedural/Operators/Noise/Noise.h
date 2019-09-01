/*=============================================================================
Noise.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_NOISE_H_
#define _PROCEDURAL_OPERATORS_NOISE_H_

#include "../Common.h"

namespace Operators
{
	class Noise1DOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(Noise1DOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(Noise1DOperator);
			SE_Field(A, real32, Public);
		SE_END_REFLECTION(Noise1DOperator);

	public:
		Noise1DOperator() : super()
		{
			A = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 A = Property("A")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(A));
		}

		real32 Do(real32 A)
		{
			return noise(A);
		}

	protected:
		real32 A;
	};

	class Noise2DOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(Noise2DOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(Noise2DOperator);
			SE_Field(A, real32, Public);
			SE_Field(B, real32, Public);
		SE_END_REFLECTION(Noise2DOperator);

	public:
		Noise2DOperator() : super()
		{
			A = 0.0f;
			B = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 A = Property("A")->GetValue().ToReal32();
			real32 B = Property("B")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(A, B));
		}

		real32 Do(real32 A, real32 B)
		{
			return CellNoise(A, B, 0.0);
		}

	protected:
		real32 A;
		real32 B;
	};

	class SNoise1DOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(SNoise1DOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(SNoise1DOperator);
			SE_Field(A, real32, Public);
		SE_END_REFLECTION(SNoise1DOperator);

	public:
		SNoise1DOperator() : super()
		{
			A = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 A = Property("A")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(A));
		}

		real32 Do(real32 A)
		{
			return (2.0f * noise(A) - 1.0f);
		}

	protected:
		real32 A;
	};

	class SNoise2DOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(SNoise2DOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(SNoise2DOperator);
			SE_Field(A, real32, Public);
			SE_Field(B, real32, Public);
		SE_END_REFLECTION(SNoise2DOperator);

	public:
		SNoise2DOperator() : super()
		{
			A = 0.0f;
			B = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 A = Property("A")->GetValue().ToReal32();
			real32 B = Property("B")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(A, B));
		}

		real32 Do(real32 A, real32 B)
		{
			return (2.0f * noise(A, B) - 1.0f);
		}

	protected:
		real32 A;
		real32 B;
	};

	class TurbulenceOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(TurbulenceOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(TurbulenceOperator);
			SE_Field(P, Vector3, Public);
			SE_Field(Octaves, int32, Public);
			SE_Field(Frequence, real32, Public);
		SE_END_REFLECTION(TurbulenceOperator);

	public:
		TurbulenceOperator() : super()
		{
			P = Vector3::Zero;
			Octaves = 5;
			Frequence = 0.5f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			Vector3 P = VariantToVector3(Property("P")->GetValue());
			int32 Octaves = Property("Octaves")->GetValue().ToInt32();
			real32 Frequence = Property("Frequence")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(P, Octaves, Frequence));
		}

		real32 Do(Vector3 P, int32 Octaves, real32 Frequence)
		{
			return PerlinNoise3D(P.X, P.Y, P.Z, 2.0f, Frequence, Octaves);
		}

	protected:
		Vector3 P;
		int32 Octaves;
		real32 Frequence;
	};

	SE_IMPLEMENT_CLASS(Noise1DOperator);
	SE_IMPLEMENT_REFLECTION(Noise1DOperator);

	SE_IMPLEMENT_CLASS(Noise2DOperator);
	SE_IMPLEMENT_REFLECTION(Noise2DOperator);

	SE_IMPLEMENT_CLASS(SNoise1DOperator);
	SE_IMPLEMENT_REFLECTION(SNoise1DOperator);

	SE_IMPLEMENT_CLASS(SNoise2DOperator);
	SE_IMPLEMENT_REFLECTION(SNoise2DOperator);

	SE_IMPLEMENT_CLASS(TurbulenceOperator);
	SE_IMPLEMENT_REFLECTION(TurbulenceOperator);
}

#endif 

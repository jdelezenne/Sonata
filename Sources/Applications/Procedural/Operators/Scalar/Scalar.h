/*=============================================================================
Scalar.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_SCALAR_H_
#define _PROCEDURAL_OPERATORS_SCALAR_H_

#include "../Common.h"

namespace Operators
{
	class ScalarAddOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarAddOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarAddOperator);
			SE_Field(ScalarA, real32, Public);
			SE_Field(ScalarB, real32, Public);
		SE_END_REFLECTION(ScalarAddOperator);

	public:
		ScalarAddOperator() : super()
		{
			ScalarA = 0.5f;
			ScalarB = 0.5f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 a = Property("ScalarA")->GetValue().ToReal32();
			real32 b = Property("ScalarB")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(a, b));
		}

		real32 Do(real32 a, real32 b)
		{
			return (a + b);
		}

	protected:
		real32 ScalarA;
		real32 ScalarB;
	};

	class ScalarDivOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarDivOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarDivOperator);
			SE_Field(ScalarA, real32, Public);
			SE_Field(ScalarB, real32, Public);
		SE_END_REFLECTION(ScalarDivOperator);

	public:
		ScalarDivOperator() : super()
		{
			ScalarA = 0.5f;
			ScalarB = 0.5f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 a = Property("ScalarA")->GetValue().ToReal32();
			real32 b = Property("ScalarB")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(a, b));
		}

		real32 Do(real32 a, real32 b)
		{
			return (b != 0.0f ? a / b : 0.0f);
		}

	protected:
		real32 ScalarA;
		real32 ScalarB;
	};

	class ScalarInvertOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarInvertOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarInvertOperator);
			SE_Field(Scalar, real32, Public);
		SE_END_REFLECTION(ScalarInvertOperator);

	public:
		ScalarInvertOperator() : super()
		{
			Scalar = 1.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 value = Property("Scalar")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(value));
		}

		real32 Do(real32 value)
		{
			return (1.0f - value);
		}

	protected:
		real32 Scalar;
	};

	class ScalarLerpOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarLerpOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarLerpOperator);
			SE_Field(ScalarA, real32, Public);
			SE_Field(ScalarB, real32, Public);
			SE_Field(X, real32, Public);
		SE_END_REFLECTION(ScalarLerpOperator);

	public:
		ScalarLerpOperator() : super()
		{
			ScalarA = 0.0f;
			ScalarB = 1.0f;
			X = 0.5f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 a = Property("ScalarA")->GetValue().ToReal32();
			real32 b = Property("ScalarB")->GetValue().ToReal32();
			real32 x = Property("X")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(a, b, x));
		}

		real32 Do(real32 a, real32 b, real32 x)
		{
			return mix(a, b, x);
		}

	protected:
		real32 ScalarA;
		real32 ScalarB;
		real32 X;
	};

	class ScalarMulOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarMulOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarMulOperator);
			SE_Field(ScalarA, real32, Public);
			SE_Field(ScalarB, real32, Public);
		SE_END_REFLECTION(ScalarMulOperator);

	public:
		ScalarMulOperator() : super()
		{
			ScalarA = 0.5f;
			ScalarB = 0.5f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 a = Property("ScalarA")->GetValue().ToReal32();
			real32 b = Property("ScalarB")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(a, b));
		}

		real32 Do(real32 a, real32 b)
		{
			return (a * b);
		}

	protected:
		real32 ScalarA;
		real32 ScalarB;
	};

	class ScalarNegateOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarNegateOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarNegateOperator);
			SE_Field(Scalar, real32, Public);
		SE_END_REFLECTION(ScalarNegateOperator);

	public:
		ScalarNegateOperator() : super()
		{
			Scalar = 1.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 value = Property("Scalar")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(value));
		}

		real32 Do(real32 value)
		{
			return -value;
		}

	protected:
		real32 Scalar;
	};

	class ScalarSubOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarSubOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarSubOperator);
			SE_Field(ScalarA, real32, Public);
			SE_Field(ScalarB, real32, Public);
		SE_END_REFLECTION(ScalarSubOperator);

	public:
		ScalarSubOperator() : super()
		{
			ScalarA = 1.0f;
			ScalarB = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 a = Property("ScalarA")->GetValue().ToReal32();
			real32 b = Property("ScalarB")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(a, b));
		}

		real32 Do(real32 a, real32 b)
		{
			return (a - b);
		}

	protected:
		real32 ScalarA;
		real32 ScalarB;
	};

	class ScalarValueOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarValueOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarValueOperator);
			SE_Field(Value, real32, Public);
		SE_END_REFLECTION(ScalarValueOperator);

	public:
		ScalarValueOperator() : super()
		{
			Value = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			Slot("Output")->SetValue(Property("Value")->GetValue().ToReal32());
		}

	protected:
		real32 Value;
	};

	class FunctionOperator : public FunctionOperatorBase
	{
		SE_DECLARE_CLASS(FunctionOperator, FunctionOperatorBase);

		SE_BEGIN_REFLECTION(FunctionOperator);
			SE_Field(Value, real32, Public);
		SE_END_REFLECTION(FunctionOperator);

	public:
		FunctionOperator() : super()
		{
			Value = 0.0f;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			Slot("Output")->SetValue(Property("Value")->GetValue().ToReal32());
		}

	protected:
		real32 Value;
	};

	DECLARE_OPERATOR_SCALAR2(ScalarAvg, ((A+B)/2.0f), A, B);
	DECLARE_OPERATOR_SCALAR3(ScalarContrast, ((A+Shift)*Gain), A, Shift, Gain);
	DECLARE_OPERATOR_SCALAR2(Gamma, (Math::Pow(X, 1.0f/G)), X, G);
	DECLARE_OPERATOR_SCALAR1(ScalarHalf, (A/2.0f), A);
	DECLARE_OPERATOR_SCALAR1(ScalarShift, ((A+1.0f)/2.0f), A);

	SE_IMPLEMENT_CLASS(ScalarAddOperator);
	SE_IMPLEMENT_REFLECTION(ScalarAddOperator);

	SE_IMPLEMENT_CLASS(ScalarDivOperator);
	SE_IMPLEMENT_REFLECTION(ScalarDivOperator);

	SE_IMPLEMENT_CLASS(ScalarInvertOperator);
	SE_IMPLEMENT_REFLECTION(ScalarInvertOperator);

	SE_IMPLEMENT_CLASS(ScalarLerpOperator);
	SE_IMPLEMENT_REFLECTION(ScalarLerpOperator);

	SE_IMPLEMENT_CLASS(ScalarMulOperator);
	SE_IMPLEMENT_REFLECTION(ScalarMulOperator);

	SE_IMPLEMENT_CLASS(ScalarNegateOperator);
	SE_IMPLEMENT_REFLECTION(ScalarNegateOperator);

	SE_IMPLEMENT_CLASS(ScalarSubOperator);
	SE_IMPLEMENT_REFLECTION(ScalarSubOperator);

	SE_IMPLEMENT_CLASS(ScalarValueOperator);
	SE_IMPLEMENT_REFLECTION(ScalarValueOperator);

	SE_IMPLEMENT_CLASS(FunctionOperator);
	SE_IMPLEMENT_REFLECTION(FunctionOperator);
}

#endif 

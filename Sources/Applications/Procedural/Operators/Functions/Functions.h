/*=============================================================================
Functions.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_FUNCTIONS_H_
#define _PROCEDURAL_OPERATORS_FUNCTIONS_H_

#include "../Common.h"

namespace Operators
{
	DECLARE_OPERATOR_SCALAR1(Abs, Math::Abs(X), X);
	DECLARE_OPERATOR_SCALAR1(Acos, Math::Acos(A), A);
	DECLARE_OPERATOR_SCALAR1(Asin, Math::Asin(A), A);
	DECLARE_OPERATOR_SCALAR1(Atan, Math::Atan(A), A);
	DECLARE_OPERATOR_SCALAR1(Ceil, Math::Ceiling(X), X);
	DECLARE_OPERATOR_SCALAR3(Clamp, Math::Clamp(X, Min, Max), X, Min, Max);
	DECLARE_OPERATOR_SCALAR1(Cos, Math::Cos(A), A);
	DECLARE_OPERATOR_SCALAR1(Exp, Math::Exp(X), X);
	DECLARE_OPERATOR_SCALAR1(Floor, Math::Floor(X), X);
	DECLARE_OPERATOR_SCALAR2(Log, Math::Log(X), X, Base);
	DECLARE_OPERATOR_SCALAR2(Max, Math::Max(A, B), A, B);
	DECLARE_OPERATOR_SCALAR2(Min, Math::Min(A, B), A, B);
	DECLARE_OPERATOR_SCALAR2(Mod, Math::Mod(A, B), A, B);
	DECLARE_OPERATOR_SCALAR2(Pow, Math::Pow(X, Y), X, Y);
	DECLARE_OPERATOR_SCALAR0(Random, Math::Random(0.0f, 1.0f));
	DECLARE_OPERATOR_SCALAR1(Round, Math::Round(X), X);
	DECLARE_OPERATOR_SCALAR1(Sign, Math::Sign(X), X);
	DECLARE_OPERATOR_SCALAR1(Sin, Math::Sin(A), A);
	DECLARE_OPERATOR_SCALAR1(Sqrt, Math::Sqrt(X), X);
	DECLARE_OPERATOR_SCALAR1(Tan, Math::Tan(A), A);

	class STransformOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(STransformOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(STransformOperator);
			SE_Field(Scale, real32, Public);
			SE_Field(Offset, real32, Public);
			SE_Field(Rotation, real32, Public);
		SE_END_REFLECTION(STransformOperator);

	public:
		STransformOperator() : super()
		{
			Scale = 1.0f;
			Offset = 0.0f;
			Rotation = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 Scale = Property("Scale")->GetValue().ToReal32();
			real32 Offset = Property("Offset")->GetValue().ToReal32();
			real32 Rotation = Property("Rotation")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(Scale, Offset, Rotation));
		}

		real32 Do(real32 Scale, real32 Offset, real32 Rotation)
		{
			GET_ST();
			real32 ss = s;
			if (Rotation != 0.0f)
				ss = s*Math::Cos(Rotation) - t*Math::Sin(Rotation);
			return Math::Mod((ss* Scale) + Offset, 1.0f) ;
		}

	protected:
		real32 Scale;
		real32 Offset;
		real32 Rotation;
	};

	class TTransformOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(TTransformOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(TTransformOperator);
			SE_Field(Scale, real32, Public);
			SE_Field(Offset, real32, Public);
			SE_Field(Rotation, real32, Public);
		SE_END_REFLECTION(TTransformOperator);

	public:
		TTransformOperator() : super()
		{
			Scale = 1.0f;
			Offset = 0.0f;
			Rotation = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			real32 Scale = Property("Scale")->GetValue().ToReal32();
			real32 Offset = Property("Offset")->GetValue().ToReal32();
			real32 Rotation = Property("Rotation")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(Scale, Offset, Rotation));
		}

		real32 Do(real32 Scale, real32 Offset, real32 Rotation)
		{
			GET_ST();
			real32 tt = t;
			if (Rotation != 0.0f)
				tt = s*Math::Sin(Rotation) + t*Math::Cos(Rotation);
			return Math::Mod((tt* Scale) + Offset, 1.0f) ;
		}

	protected:
		real32 Scale;
		real32 Offset;
		real32 Rotation;
	};

	SE_IMPLEMENT_CLASS(STransformOperator);
	SE_IMPLEMENT_REFLECTION(STransformOperator);

	SE_IMPLEMENT_CLASS(TTransformOperator);
	SE_IMPLEMENT_REFLECTION(TTransformOperator);
}

#endif 

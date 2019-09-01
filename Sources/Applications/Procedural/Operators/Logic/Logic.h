/*=============================================================================
Logic.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_LOGIC_H_
#define _PROCEDURAL_OPERATORS_LOGIC_H_

#include "../Common.h"

namespace Operators
{
	class IfColorOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(IfColorOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(IfColorOperator);
			SE_Field(Condition, bool, Public);
			SE_Field(True, Color8, Public);
			SE_Field(False, Color8, Public);
		SE_END_REFLECTION(IfColorOperator);

	public:
		IfColorOperator() : super()
		{
			Condition = false;
			True = Color8::Black;
			False = Color8::White;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			bool Condition = Property("Condition")->GetValue().ToBoolean();
			Color8 True = VariantToColor8(Property("True")->GetValue());
			Color8 False = VariantToColor8(Property("False")->GetValue());
			Slot("Output")->SetValue(ToVariant(Do(Condition, True, False)));
		}

		Color8 Do(bool Condition, Color8 True, Color8 False)
		{
			return (Condition ? True : False);
		}

	protected:
		bool Condition;
		Color8 True;
		Color8 False;
	};

	class IfScalarOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(IfScalarOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(IfScalarOperator);
			SE_Field(Condition, bool, Public);
			SE_Field(True, real32, Public);
			SE_Field(False, real32, Public);
		SE_END_REFLECTION(IfScalarOperator);

	public:
		IfScalarOperator() : super()
		{
			Condition = false;
			True = 0.0f;
			False = 1.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			bool Condition = Property("Condition")->GetValue().ToBoolean();
			real32 True = Property("True")->GetValue().ToReal32();
			real32 False = Property("False")->GetValue().ToReal32();
			Slot("Output")->SetValue(Do(Condition, True, False));
		}

		real32 Do(bool Condition, real32 True, real32 False)
		{
			return (Condition ? True : False);
		}

	protected:
		bool Condition;
		real32 True;
		real32 False;
	};

	SE_IMPLEMENT_CLASS(IfColorOperator);
	SE_IMPLEMENT_REFLECTION(IfColorOperator);

	SE_IMPLEMENT_CLASS(IfScalarOperator);
	SE_IMPLEMENT_REFLECTION(IfScalarOperator);

	DECLARE_OPERATOR_BOOL2(And, (A & B), A, B);
	DECLARE_OPERATOR_BOOL1(Not, (!A), A);
	DECLARE_OPERATOR_BOOL2(Or, (A | B), A, B);
	DECLARE_OPERATOR_BOOL2(Xor, (A ^ B), A, B);
	DECLARE_OPERATOR_BOOL2(Equal, (A == B), A, B);
	DECLARE_OPERATOR_BOOL2(NotEqual, (A != B), A, B);
	DECLARE_OPERATOR_BOOL_SCALAR2(EqualScalar, (A == B), A, B);
	DECLARE_OPERATOR_BOOL_SCALAR2(NotEqualScalar, (A != B), A, B);
	DECLARE_OPERATOR_BOOL_SCALAR2(Less, (A < B), A, B);
	DECLARE_OPERATOR_BOOL_SCALAR2(LessOrEqual, (A <= B), A, B);
	DECLARE_OPERATOR_BOOL_SCALAR2(Greater, (A > B), A, B);
	DECLARE_OPERATOR_BOOL_SCALAR2(GreaterOrEqual, (A >= B), A, B);
}

#endif 

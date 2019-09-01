/*=============================================================================
Globals.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_GLOBALS_H_
#define _PROCEDURAL_OPERATORS_GLOBALS_H_

#include "../Common.h"

namespace Operators
{
	class SOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(SOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(SOperator);
		SE_END_REFLECTION(SOperator);

	public:
		SOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			GET_ST();
			Slot("Output")->SetValue(s);
		}
	};

	class TOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(TOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(TOperator);
		SE_END_REFLECTION(TOperator);

	public:
		TOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			GET_ST();
			Slot("Output")->SetValue(t);
		}
	};

	class XOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(XOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(XOperator);
		SE_END_REFLECTION(XOperator);

	public:
		XOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, int32);
		}

		virtual void Update()
		{
			GET_ST();
			GET_W();
			Slot("Output")->SetValue((int32)(s*w));
		}
	};

	class YOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(YOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(YOperator);
		SE_END_REFLECTION(YOperator);

	public:
		YOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, int32);
		}

		virtual void Update()
		{
			GET_ST();
			GET_H();
			Slot("Output")->SetValue((int32)(t*h));
		}
	};

	class WOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(WOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(WOperator);
		SE_END_REFLECTION(WOperator);

	public:
		WOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, int32);
		}

		virtual void Update()
		{
			GET_W();
			Slot("Output")->SetValue(w);
		}
	};

	class HOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(HOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(HOperator);
		SE_END_REFLECTION(HOperator);

	public:
		HOperator() : super()
		{
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, int32);
		}

		virtual void Update()
		{
			GET_H();
			Slot("Output")->SetValue(h);
		}
	};

	SE_IMPLEMENT_CLASS(SOperator);
	SE_IMPLEMENT_REFLECTION(SOperator);

	SE_IMPLEMENT_CLASS(TOperator);
	SE_IMPLEMENT_REFLECTION(TOperator);

	SE_IMPLEMENT_CLASS(XOperator);
	SE_IMPLEMENT_REFLECTION(XOperator);

	SE_IMPLEMENT_CLASS(YOperator);
	SE_IMPLEMENT_REFLECTION(YOperator);

	SE_IMPLEMENT_CLASS(WOperator);
	SE_IMPLEMENT_REFLECTION(WOperator);

	SE_IMPLEMENT_CLASS(HOperator);
	SE_IMPLEMENT_REFLECTION(HOperator);

	DECLARE_OPERATOR_SCALAR0(Pi, Math::Pi);
	DECLARE_OPERATOR_SCALAR0(TwoPi, Math::TwoPi);
	DECLARE_OPERATOR_SCALAR0(HalfPi, Math::HalfPi);
	DECLARE_OPERATOR_SCALAR0(InvPi, Math::InvPi);
	DECLARE_OPERATOR_SCALAR0(PiBy180, Math::PiBy180);
}

#endif 

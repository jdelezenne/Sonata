/*=============================================================================
Operators.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_H_
#define _PROCEDURAL_OPERATORS_H_

#include "../Workflow.h"
#include "../Utils.h"

#define GET_ST() \
	real32 s, t; \
	_Workflow->GetST(s, t);

#define GET_STEP() \
	real32 ss, tt; \
	GET_W(); \
	GET_H(); \
	ss = 1.0f / w; \
	tt = 1.0f / h;

#define SET_ST(s, t) \
	_Workflow->SetST(s, t);

#define GET_W() \
	int32 w; \
	w = _Workflow->GetWidth();

#define GET_H() \
	int32 h; \
	h = _Workflow->GetHeight();

#define DECLARE_OPERATOR_SCALAR0(name, function) \
	class name##Operator : public PrimitiveOperatorBase \
	{ \
		SE_DECLARE_CLASS(name##Operator, PrimitiveOperatorBase); \
 \
		SE_BEGIN_REFLECTION(name##Operator); \
		SE_END_REFLECTION(name##Operator); \
 \
	public: \
		name##Operator() : super() \
		{ \
		} \
 \
		virtual void Create() \
		{ \
			super::Create(); \
 \
			DECLARE_COMPONENTSLOT(Output, real32); \
		} \
 \
		virtual void Update() \
		{ \
			Slot("Output")->SetValue(function); \
		} \
	}; \
 \
	SE_IMPLEMENT_CLASS(name##Operator); \
	SE_IMPLEMENT_REFLECTION(name##Operator);

#define DECLARE_OPERATOR_SCALAR1(name, function, var) \
	class name##Operator : public PrimitiveOperatorBase \
	{ \
		SE_DECLARE_CLASS(name##Operator, PrimitiveOperatorBase); \
 \
		SE_BEGIN_REFLECTION(name##Operator); \
			SE_Field(var, real32, Public); \
		SE_END_REFLECTION(name##Operator); \
 \
	public: \
		name##Operator() : super() \
		{ \
			var = 0.0f; \
		} \
 \
		virtual void Create() \
		{ \
			super::Create(); \
 \
			DECLARE_COMPONENTSLOT(Output, real32); \
		} \
 \
		virtual void Update() \
		{ \
			real32 var = Property(#var)->GetValue().ToReal32(); \
			Slot("Output")->SetValue(Do(var)); \
		} \
 \
		real32 Do(real32 var) \
		{ \
			return (function); \
		} \
 \
	protected: \
		real32 var; \
	}; \
 \
	SE_IMPLEMENT_CLASS(name##Operator); \
	SE_IMPLEMENT_REFLECTION(name##Operator);

#define DECLARE_OPERATOR_SCALAR2(name, function, varA, varB) \
	class name##Operator : public PrimitiveOperatorBase \
	{ \
		SE_DECLARE_CLASS(name##Operator, PrimitiveOperatorBase); \
 \
		SE_BEGIN_REFLECTION(name##Operator); \
			SE_Field(varA, real32, Public); \
			SE_Field(varB, real32, Public); \
		SE_END_REFLECTION(name##Operator); \
 \
	public: \
		name##Operator() : super() \
		{ \
			varA = 0.0f; \
			varB = 0.0f; \
		} \
 \
		virtual void Create() \
		{ \
			super::Create(); \
 \
			DECLARE_COMPONENTSLOT(Output, real32); \
		} \
 \
		virtual void Update() \
		{ \
			real32 varA = Property(#varA)->GetValue().ToReal32(); \
			real32 varB = Property(#varB)->GetValue().ToReal32(); \
			Slot("Output")->SetValue(Do(varA, varB)); \
		} \
 \
		real32 Do(real32 varA, real32 varB) \
		{ \
			return (function); \
		} \
 \
	protected: \
		real32 varA; \
		real32 varB; \
	}; \
 \
	SE_IMPLEMENT_CLASS(name##Operator); \
	SE_IMPLEMENT_REFLECTION(name##Operator);

#define DECLARE_OPERATOR_SCALAR3(name, function, varA, varB, varC) \
	class name##Operator : public PrimitiveOperatorBase \
	{ \
		SE_DECLARE_CLASS(name##Operator, PrimitiveOperatorBase); \
 \
		SE_BEGIN_REFLECTION(name##Operator); \
			SE_Field(varA, real32, Public); \
			SE_Field(varB, real32, Public); \
			SE_Field(varC, real32, Public); \
		SE_END_REFLECTION(name##Operator); \
 \
	public: \
		name##Operator() : super() \
		{ \
			varA = 0.0f; \
			varB = 0.0f; \
			varC = 0.0f; \
		} \
 \
		virtual void Create() \
		{ \
			super::Create(); \
 \
			DECLARE_COMPONENTSLOT(Output, real32); \
		} \
 \
		virtual void Update() \
		{ \
			real32 varA = Property(#varA)->GetValue().ToReal32(); \
			real32 varB = Property(#varB)->GetValue().ToReal32(); \
			real32 varC = Property(#varC)->GetValue().ToReal32(); \
			Slot("Output")->SetValue(Do(varA, varB, varC)); \
		} \
 \
		real32 Do(real32 varA, real32 varB, real32 varC) \
		{ \
			return (function); \
		} \
 \
	protected: \
		real32 varA; \
		real32 varB; \
		real32 varC; \
	}; \
 \
	SE_IMPLEMENT_CLASS(name##Operator); \
	SE_IMPLEMENT_REFLECTION(name##Operator);


#define DECLARE_OPERATOR_BOOL1(name, function, var) \
	class name##Operator : public PrimitiveOperatorBase \
	{ \
		SE_DECLARE_CLASS(name##Operator, PrimitiveOperatorBase); \
 \
		SE_BEGIN_REFLECTION(name##Operator); \
			SE_Field(var, bool, Public); \
		SE_END_REFLECTION(name##Operator); \
 \
	public: \
		name##Operator() : super() \
		{ \
			var = false; \
		} \
 \
		virtual void Create() \
		{ \
			super::Create(); \
 \
			DECLARE_COMPONENTSLOT(Output, bool); \
		} \
 \
		virtual void Update() \
		{ \
			bool var = Property(#var)->GetValue().ToBoolean(); \
			Slot("Output")->SetValue(Do(var)); \
		} \
 \
		bool Do(bool var) \
		{ \
			return (function); \
		} \
 \
	protected: \
		bool var; \
	}; \
 \
	SE_IMPLEMENT_CLASS(name##Operator); \
	SE_IMPLEMENT_REFLECTION(name##Operator);

#define DECLARE_OPERATOR_BOOL2(name, function, varA, varB) \
	class name##Operator : public PrimitiveOperatorBase \
	{ \
		SE_DECLARE_CLASS(name##Operator, PrimitiveOperatorBase); \
 \
		SE_BEGIN_REFLECTION(name##Operator); \
			SE_Field(varA, bool, Public); \
			SE_Field(varB, bool, Public); \
		SE_END_REFLECTION(name##Operator); \
 \
	public: \
		name##Operator() : super() \
		{ \
			varA = false; \
			varB = false; \
		} \
 \
		virtual void Create() \
		{ \
			super::Create(); \
 \
			DECLARE_COMPONENTSLOT(Output, bool); \
		} \
 \
		virtual void Update() \
		{ \
			bool varA = Property(#varA)->GetValue().ToBoolean(); \
			bool varB = Property(#varB)->GetValue().ToBoolean(); \
			Slot("Output")->SetValue(Do(varA, varB)); \
		} \
 \
		real32 Do(bool varA, bool varB) \
		{ \
			return (function); \
		} \
 \
	protected: \
		bool varA; \
		bool varB; \
	}; \
 \
	SE_IMPLEMENT_CLASS(name##Operator); \
	SE_IMPLEMENT_REFLECTION(name##Operator);

#define DECLARE_OPERATOR_BOOL_SCALAR2(name, function, varA, varB) \
	class name##Operator : public PrimitiveOperatorBase \
	{ \
		SE_DECLARE_CLASS(name##Operator, PrimitiveOperatorBase); \
 \
		SE_BEGIN_REFLECTION(name##Operator); \
			SE_Field(varA, real32, Public); \
			SE_Field(varB, real32, Public); \
		SE_END_REFLECTION(name##Operator); \
 \
	public: \
		name##Operator() : super() \
		{ \
			varA = 0.0f; \
			varB = 0.0f; \
		} \
 \
		virtual void Create() \
		{ \
			super::Create(); \
 \
			DECLARE_COMPONENTSLOT(Output, bool); \
		} \
 \
		virtual void Update() \
		{ \
			real32 varA = Property(#varA)->GetValue().ToReal32(); \
			real32 varB = Property(#varB)->GetValue().ToReal32(); \
			Slot("Output")->SetValue(Do(varA, varB)); \
		} \
 \
		bool Do(real32 varA, real32 varB) \
		{ \
			return (function); \
		} \
 \
	protected: \
		real32 varA; \
		real32 varB; \
	}; \
 \
	SE_IMPLEMENT_CLASS(name##Operator); \
	SE_IMPLEMENT_REFLECTION(name##Operator);

namespace Operators
{
	class MultipleOperatorBase : public Operator
	{
		SE_DECLARE_ABSTRACT(MultipleOperatorBase, Operator);

	public:
		MultipleOperatorBase();

		virtual void Create();

		virtual Variant GetSlotValue(const String& name);
		virtual void SetSlotValue(const String& name, const Variant& value);

		virtual void DoRefresh();

		virtual UI::TableNode* CreateNode();

	protected:
		Dictionary<String, Variant> _Values;
	};


	class PrimitiveOperatorBase : public Operator
	{
		SE_DECLARE_ABSTRACT(PrimitiveOperatorBase, Operator);

	public:
		PrimitiveOperatorBase();

		virtual void Create();

		virtual Variant GetSlotValue(const String& name);
		virtual void SetSlotValue(const String& name, const Variant& value);

		virtual void DoRefresh();

		virtual UI::TableNode* CreateNode();

		const Variant GetValue() const { return _Value; }

	protected:
		Variant _Value;
	};


	class ColorOperatorBase : public Operator
	{
		SE_DECLARE_ABSTRACT(ColorOperatorBase, Operator);

	public:
		ColorOperatorBase();

		virtual void Create();

		virtual Variant GetSlotValue(const String& name);
		virtual void SetSlotValue(const String& name, const Variant& value);

		Color32 GetColorAt(real32 s, real32 t)
		{
			SET_ST(s, t);
			return VariantToColor32(Property("Color")->GetValue());
		}

		virtual void DoRefresh();

		virtual UI::TableNode* CreateNode();

		const Image* GetImage() const { return &_Image; }

	protected:
		Image _Image;
	};


	class FunctionOperatorBase : public Operator
	{
		SE_DECLARE_ABSTRACT(FunctionOperatorBase, Operator);

	public:
		FunctionOperatorBase();

		virtual void Create();

		virtual Variant GetSlotValue(const String& name);
		virtual void SetSlotValue(const String& name, const Variant& value);

		virtual void DoRefresh();

		virtual UI::TableNode* CreateNode();

		const Variant GetValue(int32 index) const { return _Values[index]; }
		const Image* GetImage() const { return &_Image; }

	protected:
		Array<Variant> _Values;
		Image _Image;
	};


	class ImageOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(ImageOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(ImageOperator);
			SE_Field(FileName, String, Public);
		SE_END_REFLECTION(ImageOperator);

	public:
		ImageOperator() : super()
		{
			_SourceImage = NULL;
		}

		virtual void DoRefresh()
		{
			SE_DELETE(_SourceImage);
			if (!FileName.IsEmpty())
			{
				_SourceImage = CreateImage(FileName);
			}

			super::DoRefresh();
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			if (FileName.IsEmpty() || _SourceImage == NULL)
			{
				Slot("Output")->SetValue(ToVariant(Color8::Black));
			}
			else
			{
				GET_ST();
				Color8 color = _SourceImage->GetRGB(
					s * _SourceImage->GetWidth(),
					t * _SourceImage->GetHeight());

				Slot("Output")->SetValue(ToVariant(color));
			}
		}

	public:
		String FileName;

	protected:
		Image* _SourceImage;
	};
}

#endif 

/*=============================================================================
Conversions.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_CONVERSIONS_H_
#define _PROCEDURAL_OPERATORS_CONVERSIONS_H_

#include "../Common.h"

namespace Operators
{
	class ColorToRGBOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(ColorToRGBOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(ColorToRGBOperator);
			SE_Field(Color, Color8, Public);
		SE_END_REFLECTION(ColorToRGBOperator);

	public:
		ColorToRGBOperator() : super()
		{
			Color = Color8::White;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Red, real32);
			DECLARE_COMPONENTSLOT(Green, real32);
			DECLARE_COMPONENTSLOT(Blue, real32);
		}

		virtual void Update()
		{
			Color32 color = VariantToColor32(Property("Color")->GetValue());
			Slot("Red")->SetValue(color.r);
			Slot("Green")->SetValue(color.g);
			Slot("Blue")->SetValue(color.b);
		}

	protected:
		Color8 Color;
	};

	class RGBToColorOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(RGBToColorOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(RGBToColorOperator);
			SE_Field(Red, uint8, Public);
			SE_Field(Green, uint8, Public);
			SE_Field(Blue, uint8, Public);
		SE_END_REFLECTION(RGBToColorOperator);

	public:
		RGBToColorOperator() : super()
		{
			Red = 255;
			Green = 255;
			Blue = 255;
		}

		virtual void Create()
		{
			super::Create();
		}

		virtual void Update()
		{
			real32 r = GetColor(Property("Red"));
			real32 g = GetColor(Property("Green"));
			real32 b = GetColor(Property("Blue"));
			Slot("Output")->SetValue(ToVariant(Color32::Clamp(Color32(r, g, b), 0.0f, 1.0f)));
		}

	protected:
		real32 GetColor(OperatorProperty* p)
		{
			if (!p->IsLinked())
				return (p->GetValue().ToUInt8() / 255.0f);
			else
				return p->GetValue().ToReal32();
		}

		uint8 Red;
		uint8 Green;
		uint8 Blue;
	};

	class VectorToXYZOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(VectorToXYZOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(VectorToXYZOperator);
			SE_Field(Vector, Vector3, Public);
		SE_END_REFLECTION(VectorToXYZOperator);

	public:
		VectorToXYZOperator() : super()
		{
			Vector = Vector3::Zero;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(X, real32);
			DECLARE_COMPONENTSLOT(Y, real32);
			DECLARE_COMPONENTSLOT(Z, real32);
		}

		virtual void Update()
		{
			Vector3 vector = VariantToVector3(Property("Vector")->GetValue());
			Slot("X")->SetValue(vector.X);
			Slot("Y")->SetValue(vector.Y);
			Slot("Z")->SetValue(vector.Z);
		}

	protected:
		Vector3 Vector;
	};

	class XYZToVectorOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(XYZToVectorOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(XYZToVectorOperator);
			SE_Field(X, real32, Public);
			SE_Field(Y, real32, Public);
			SE_Field(Z, real32, Public);
		SE_END_REFLECTION(XYZToVectorOperator);

	public:
		XYZToVectorOperator() : super()
		{
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Vector3);
		}

		virtual void Update()
		{
			real32 x = Property("X")->GetValue().ToReal32();
			real32 y = Property("Y")->GetValue().ToReal32();
			real32 z = Property("Z")->GetValue().ToReal32();
			Slot("Output")->SetValue(ToVariant(Vector3::Clamp(Vector3(x, y, z), 0.0f, 1.0f)));
		}

	protected:
		real32 X;
		real32 Y;
		real32 Z;
	};

	class ColorToVectorOperator : public MultipleOperatorBase
	{
		SE_DECLARE_CLASS(ColorToVectorOperator, MultipleOperatorBase);

		SE_BEGIN_REFLECTION(ColorToVectorOperator);
			SE_Field(Color, Color32, Public);
		SE_END_REFLECTION(ColorToVectorOperator);

	public:
		ColorToVectorOperator() : super()
		{
			Color = Color32::White;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Vector3);
		}

		virtual void Update()
		{
			Color32 color = VariantToColor32(Property("Color")->GetValue());
			Slot("Output")->SetValue(ToVariant(Vector3(color.r, color.g, color.b)));
		}

	protected:
		Color32 Color;
	};

	class VectorToColorOperator : public ColorOperatorBase
	{
		SE_DECLARE_CLASS(VectorToColorOperator, ColorOperatorBase);

		SE_BEGIN_REFLECTION(VectorToColorOperator);
			SE_Field(Vector, Vector3, Public);
		SE_END_REFLECTION(VectorToColorOperator);

	public:
		VectorToColorOperator() : super()
		{
			Vector = Vector3::Zero;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, Color32);
		}

		virtual void Update()
		{
			Vector3 vector = VariantToVector3(Property("Vector")->GetValue());
			Slot("Output")->SetValue(ToVariant(Color32(vector.X, vector.Y, vector.Z)));
		}

	protected:
		Vector3 Vector;
	};

	class ScalarToBoolOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(ScalarToBoolOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(ScalarToBoolOperator);
			SE_Field(Scalar, real32, Public);
		SE_END_REFLECTION(ScalarToBoolOperator);

	public:
		ScalarToBoolOperator() : super()
		{
			Scalar = 0.0f;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, bool);
		}

		virtual void Update()
		{
			real32 Scalar = Property("Scalar")->GetValue().ToReal32();
			Slot("Output")->SetValue(Scalar != 0.0f);
		}

	protected:
		real32 Scalar;
	};

	class BoolToScalarOperator : public PrimitiveOperatorBase
	{
		SE_DECLARE_CLASS(BoolToScalarOperator, PrimitiveOperatorBase);

		SE_BEGIN_REFLECTION(BoolToScalarOperator);
			SE_Field(Bool, bool, Public);
		SE_END_REFLECTION(BoolToScalarOperator);

	public:
		BoolToScalarOperator() : super()
		{
			Bool = false;
		}

		virtual void Create()
		{
			super::Create();

			DECLARE_COMPONENTSLOT(Output, real32);
		}

		virtual void Update()
		{
			bool Bool = Property("Bool")->GetValue().ToBoolean();
			Slot("Output")->SetValue(Bool ? 1.0f : 0.0f);
		}

	protected:
		bool Bool;
	};

	SE_IMPLEMENT_CLASS(ColorToRGBOperator);
	SE_IMPLEMENT_REFLECTION(ColorToRGBOperator);

	SE_IMPLEMENT_CLASS(RGBToColorOperator);
	SE_IMPLEMENT_REFLECTION(RGBToColorOperator);

	SE_IMPLEMENT_CLASS(VectorToXYZOperator);
	SE_IMPLEMENT_REFLECTION(VectorToXYZOperator);

	SE_IMPLEMENT_CLASS(XYZToVectorOperator);
	SE_IMPLEMENT_REFLECTION(XYZToVectorOperator);

	SE_IMPLEMENT_CLASS(ColorToVectorOperator);
	SE_IMPLEMENT_REFLECTION(ColorToVectorOperator);

	SE_IMPLEMENT_CLASS(VectorToColorOperator);
	SE_IMPLEMENT_REFLECTION(VectorToColorOperator);

	SE_IMPLEMENT_CLASS(ScalarToBoolOperator);
	SE_IMPLEMENT_REFLECTION(ScalarToBoolOperator);

	SE_IMPLEMENT_CLASS(BoolToScalarOperator);
	SE_IMPLEMENT_REFLECTION(BoolToScalarOperator);
}

#endif 

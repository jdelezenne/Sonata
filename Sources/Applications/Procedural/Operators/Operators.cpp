/*=============================================================================
Operators.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Operators.h"
#include "Color/Color.h"
#include "Conversions/Conversions.h"
#include "Filters/Filters.h"
#include "Functions/Functions.h"
#include "Globals/Globals.h"
#include "Gradients/Gradients.h"
#include "Logic/Logic.h"
#include "Noise/Noise.h"
#include "Patterns/Patterns.h"
#include "Scalar/Scalar.h"

namespace Operators
{
	MultipleOperatorBase::MultipleOperatorBase() :
		super()
	{
	}

	void MultipleOperatorBase::Create()
	{
		super::Create();
	}

	UI::TableNode* MultipleOperatorBase::CreateNode()
	{
		super::CreateNode();

		return _Node;
	}

	Variant MultipleOperatorBase::GetSlotValue(const String& name)
	{
		Update();
		return _Values[name];
	}

	void MultipleOperatorBase::SetSlotValue(const String& name, const Variant& value)
	{
		_Values[name] = value;
	}

	void MultipleOperatorBase::DoRefresh()
	{
		Update();
	}


	PrimitiveOperatorBase::PrimitiveOperatorBase() :
		super()
	{
	}

	void PrimitiveOperatorBase::Create()
	{
		super::Create();
	}

	UI::TableNode* PrimitiveOperatorBase::CreateNode()
	{
		super::CreateNode();

		UI::TablePreview* preview = new UI::TablePreview(_Node);
		preview->SetPreviewType(UI::TablePreviewType_Text);
		_Node->SetPreview(preview);

		return _Node;
	}

	Variant PrimitiveOperatorBase::GetSlotValue(const String& name)
	{
		Update();
		return _Value;
	}

	void PrimitiveOperatorBase::SetSlotValue(const String& name, const Variant& value)
	{
		_Value = value;
	}

	void PrimitiveOperatorBase::DoRefresh()
	{
		Update();
		_Node->GetPreview()->SetText(_Value.ToString());
	}


	ColorOperatorBase::ColorOperatorBase() :
		super()
	{
	}

	void ColorOperatorBase::Create()
	{
		super::Create();

		DECLARE_COMPONENTSLOT(Output, Color8);
	}

	UI::TableNode* ColorOperatorBase::CreateNode()
	{
		super::CreateNode();

		UI::TablePreview* preview = new UI::TablePreview(_Node);
		preview->SetPreviewType(UI::TablePreviewType_Image);
		_Image.Create(PixelFormat_R8G8B8A8, _Workflow->GetWidth(), _Workflow->GetHeight());
		preview->SetImage(&_Image);
		_Node->SetPreview(preview);

		return _Node;
	}

	Variant ColorOperatorBase::GetSlotValue(const String& name)
	{
		int32 width = _Image.GetWidth();
		int32 height = _Image.GetHeight();

		GET_ST();
		return ToVariant(_Image.GetRGB(s*width, t*height));
	}

	void ColorOperatorBase::SetSlotValue(const String& name, const Variant& value)
	{
		Color8 color = VariantToColor8(value);

		int32 width = _Image.GetWidth();
		int32 height = _Image.GetHeight();

		GET_ST();
		_Image.SetRGB(s*width, t*height, color);
	}

	void ColorOperatorBase::DoRefresh()
	{
		// Refresh the preview
		int32 width = _Image.GetWidth();
		int32 height = _Image.GetHeight();

		real32 sStep, tStep;

		sStep = 1.0f / (real32)width;
		tStep = 1.0f / (real32)height;

		real32 s, t;
		for (t=0.0f; t<1.0f; t+=tStep)
		{
			for (s=0.0f; s<1.0f; s+=sStep)
			{
				_Workflow->SetST(s, t);
				Update();
			}
		}

		_Node->GetPreview()->SetImage(NULL);
		_Node->GetPreview()->SetImage(&_Image);
	}


	FunctionOperatorBase::FunctionOperatorBase() :
		super()
	{
	}

	void FunctionOperatorBase::Create()
	{
		super::Create();

		DECLARE_COMPONENTSLOT(Output, real32);
	}

	UI::TableNode* FunctionOperatorBase::CreateNode()
	{
		super::CreateNode();

		UI::TablePreview* preview = new UI::TablePreview(_Node);
		preview->SetPreviewType(UI::TablePreviewType_Image);
		_Image.Create(PixelFormat_R8G8B8A8, _Workflow->GetWidth(), _Workflow->GetHeight());
		preview->SetImage(&_Image);
		_Node->SetPreview(preview);

		_Values.SetSize(_Workflow->GetWidth());

		return _Node;
	}

	Variant FunctionOperatorBase::GetSlotValue(const String& name)
	{
		int32 length = _Values.Count();

		GET_ST();
		return _Values[(int32)(s*length)];
	}

	void FunctionOperatorBase::SetSlotValue(const String& name, const Variant& value)
	{
		int32 length = _Values.Count();

		GET_ST();
		_Values[(int32)(s*length)] = value;
	}

	void FunctionOperatorBase::DoRefresh()
	{
		// Refresh the preview
		int32 length = _Values.Count();

		real32 sStep = 1.0f / (real32)length;

		real32 s;
		for (s=0.0f; s<1.0f; s+=sStep)
		{
			_Workflow->SetST(s, 0.0f);
			Update();
		}

		real32 min, max;
		min = max = _Values[0].ToReal32();

		for (s=0; s<length; s++)
		{
			min = Math::Min(min, _Values[s].ToReal32());
			max = Math::Max(max, _Values[s].ToReal32());
		}

		int32 width = _Image.GetWidth();
		int32 height = _Image.GetHeight();

		int32 x, y;
		for (y=0; y<height; y++)
		{
			for (x=0; x<width; x++)
			{
				_Image.SetRGB(x, y, Color8::Black);
			}
		}

		real32 halfHeight = height / 2;
		real32 scale = halfHeight * Math::Max(1.0f, max - min);
		for (s=0; s<length; s++)
		{
			real32 value = _Values[s].ToReal32();
			_Image.SetRGB(s, halfHeight - value*scale, Color8::White);
		}

		_Node->GetPreview()->SetImage(NULL);
		_Node->GetPreview()->SetImage(&_Image);
	}

	SE_IMPLEMENT_ABSTRACT(MultipleOperatorBase);
	SE_IMPLEMENT_ABSTRACT(PrimitiveOperatorBase);
	SE_IMPLEMENT_ABSTRACT(ColorOperatorBase);
	SE_IMPLEMENT_ABSTRACT(FunctionOperatorBase);

	SE_IMPLEMENT_CLASS(ImageOperator);
	SE_IMPLEMENT_REFLECTION(ImageOperator);
}

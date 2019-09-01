/*=============================================================================
Operator.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Operator.h"
#include "Procedural.h"

Variant ToVariant(const Color8& value)
{
	static Color8 r = value;
	r = value;
	return Variant((StructObject*)&r);
}

Variant ToVariant(const Color32& value)
{
	//static Color32 r = value;
	return ToVariant(Color8(value.r * 255.0f, value.g * 255.0f, value.b * 255.0f, value.a * 255.0f));
}

Variant ToVariant(const Vector3& value)
{
	static Vector3 r = value;
	r = value;
	return Variant((StructObject*)&r);
}

Color8 VariantToColor8(const Variant& value)
{
	Color8 r;
	Memory::Copy(&r, value.GetData(), sizeof(Color8));
	return r;
}

Color32 VariantToColor32(const Variant& value)
{
	//Color32 r;
	//Memory::Copy(&r, value.GetData(), sizeof(Color32));
	Color8 r = VariantToColor8(value);
	return Color32(r.r / 255.0f, r.g / 255.0f, r.b / 255.0f, r.a / 255.0f);
}

Vector3 VariantToVector3(const Variant& value)
{
	Vector3 r;
	Memory::Copy(&r, value.GetData(), sizeof(Vector3));
	return r;
}

Color8 ToColor8(const Color32& c)
{
	return Color8(c.r*255.0f, c.g*255.0f, c.b*255.0f, c.a*255.0f);
}

Color32 ToColor32(const Color8& c)
{
	return Color32(c.r/255.0f, c.g/255.0f, c.b/255.0f, c.a/255.0f);
}


OperatorSlot::OperatorSlot(Operator* owner)
{
	_Owner = owner;
}

OperatorSlot::~OperatorSlot()
{
}

Variant OperatorSlot::GetValue() const
{
	return _Owner->GetSlotValue(GetName());
}

void OperatorSlot::SetValue(const Variant& value)
{
	_Owner->SetSlotValue(GetName(), value);
}


OperatorProperty::OperatorProperty(Operator* owner, const FieldInfo* field)
{
	_Owner = owner;
	_Field = field;
	_CanLink = true;
	_Slot = NULL;
}

OperatorProperty::~OperatorProperty()
{
}

const TypeInfo* OperatorProperty::GetType() const
{
	return _Field->GetFieldType();
}

Variant OperatorProperty::GetValue() const
{
	if (_Slot == NULL)
	{
		return _Field->GetValue(_Owner);
	}
	else
	{
		return _Slot->GetValue();
	}
}

void OperatorProperty::SetValue(const Variant& value)
{
	_Field->SetValue(_Owner, value);
}

bool OperatorProperty::IsLinked() const
{
	return (_Slot != NULL);
}

void OperatorProperty::LinkTo(OperatorSlot* slot)
{
	if (_CanLink)
	{
		_Slot = slot;
	}
}

void OperatorProperty::Unlink()
{
	_Slot = NULL;
}


Operator::Operator() :
	RefObject()
{
	_Valid = false;
	_Node = NULL;
}

SE_IMPLEMENT_ABSTRACT(Operator);
SE_IMPLEMENT_REFLECTION(Operator);

Operator::~Operator()
{
	if (_Node != NULL)
	{
		_Workflow->GetDiagram()->RemoveNode(_Node);
	}
}

void Operator::OnSerialized(BinarySerializer* context, BinaryStream& stream)
{
	int count = _Properties.Count();
	stream.WriteInt32(count);
	for (int i=0; i<count; i++)
	{
	}
}

void Operator::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	SonataEngine::XMLDocument* document = element->GetOwnerDocument();
	XMLElement* array = document->CreateElement("Properties");
	int count = _Properties.Count();
	array->SetAttribute("Count", String::ToString(count));
	for (int i=0; i<count; i++)
	{
		XMLElement* item = document->CreateElement("Property");
		OperatorProperty* property = _Properties[i];
		if (property->IsLinked())
		{
			OperatorSlot* slot = property->GetSlot();

			XMLElement* elementID = document->CreateElement("ID");
			XMLText* valueID = document->CreateTextNode(String::Empty);
			valueID->SetValue(String::ToString(slot->GetOwner()->GetID()));
			elementID->AppendChild(valueID);
			item->AppendChild(elementID);

			XMLElement* elementName = document->CreateElement("Name");
			XMLText* valueName = document->CreateTextNode(String::Empty);
			valueName->SetValue(slot->GetName());
			elementName->AppendChild(valueName);
			item->AppendChild(elementName);
		}
		array->AppendChild(item);
	}
	element->AppendChild(array);

	XMLElement* elementPositionX = document->CreateElement("PositionX");
	XMLText* valuePositionX = document->CreateTextNode(String::Empty);
	valuePositionX->SetValue(String::ToString(_Node->GetLeft()));
	elementPositionX->AppendChild(valuePositionX);
	element->AppendChild(elementPositionX);

	XMLElement* elementPositionY = document->CreateElement("PositionY");
	XMLText* valuePositionY = document->CreateTextNode(String::Empty);
	valuePositionY->SetValue(String::ToString(_Node->GetTop()));
	elementPositionY->AppendChild(valuePositionY);
	element->AppendChild(elementPositionY);
}

void Operator::OnDeserialized(BinarySerializer* context, BinaryStream& stream)
{
	Create();

	int count = stream.ReadInt32();
	for (int i=0; i<count; i++)
	{
	}
}

void Operator::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	Create();

	XMLElement* array = (XMLElement*)element->GetItem("Properties");
	int count = array->GetAttribute("Count").ToInt32();
	XMLElement* item = (XMLElement*)array->GetItem("Property");
	if (count == _Properties.Count())
	{
		for (int i=0; i<count; i++)
		{
			OperatorProperty* property = _Properties[i];
			if (property->GetCanLink())
			{
				XMLText* valueID = NULL;
				XMLText* valueName = NULL;

				XMLElement* elementID = (XMLElement*)item->GetItem("ID");
				if (elementID != NULL)
					valueID = (XMLText*)elementID->GetFirstChild();

				XMLElement* elementName = (XMLElement*)item->GetItem("Name");
				if (elementName != NULL)
					valueName = (XMLText*)elementName->GetFirstChild();

				if (valueID != NULL && valueName != NULL)
				{
					Operator::SlotRef slotRef;
					slotRef._Index = i;
					slotRef._ID = valueID->GetValue().ToInt32();
					slotRef._name = valueName->GetValue();
					_SlotRefs.Add(slotRef);
				}
			}
			item = (XMLElement*)item->GetNextSibling();
		}
	}

	XMLText* valuePositionX = NULL;
	XMLText* valuePositionY = NULL;

	XMLElement* elementPositionX = (XMLElement*)element->GetItem("PositionX");
	if (elementPositionX != NULL)
		valuePositionX = (XMLText*)elementPositionX->GetFirstChild();

	XMLElement* elementPositionY = (XMLElement*)element->GetItem("PositionY");
	if (elementPositionY != NULL)
		valuePositionY = (XMLText*)elementPositionY->GetFirstChild();

	if (valuePositionX != NULL && valuePositionY != NULL)
	{
		_Position = PointInt(
			valuePositionX->GetValue().ToInt32(),
			valuePositionY->GetValue().ToInt32());
	}
}

UI::TableNode* Operator::CreateNode()
{
	_Node = new UI::TableNode();
	_Node->SetTitle(_name);
	_Node->SetUserData((Object*)this);
	_Node->SetSelectedColor(Color32::Yellow);

	OperatorPropertyList::Iterator itP = _Properties.GetIterator();
	while (itP.Next())
	{
		OperatorProperty* property = itP.Current();

		UI::TableProperty* input = new UI::TableProperty();
		input->SetUserData((Object*)property);
		input->SetPropertyName(property->GetName());
		_Node->InputProperties().Add(input);
	}

	OperatorSlotList::Iterator itS = _Slots.GetIterator();
	while (itS.Next())
	{
		OperatorSlot* slot = itS.Current();

		UI::TableProperty* output = new UI::TableProperty();
		output->SetUserData((Object*)slot);
		output->SetPropertyName(slot->GetName());
		_Node->OutputProperties().Add(output);
	}

	_Workflow->GetDiagram()->AddNode(_Node);

	return _Node;
}

OperatorSlot* Operator::Slot(const String& name)
{
	OperatorSlotList::Iterator it = _Slots.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName().CompareTo(name) == 0)
		{
			return it.Current();
		}
	}

	return NULL;
}

OperatorProperty* Operator::Property(const String& name)
{
	OperatorPropertyList::Iterator it = _Properties.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName().CompareTo(name) == 0)
		{
			return it.Current();
		}
	}

	return NULL;
}

void Operator::Invalidate()
{
	_Valid = false;
}

bool Operator::HasSource(Operator* op) const
{
	OperatorPropertyList::Iterator it = _Properties.GetIterator();
	while (it.Next())
	{
		OperatorProperty* property = it.Current();
		if (property->IsLinked())
		{
			if (property->GetSlot()->GetOwner()->HasSource(op))
				return true;
		}
	}

	return false;
}

void Operator::Refresh()
{
	if (_Valid)
		return;

	// Refresh the sources
	OperatorPropertyList::Iterator it = _Properties.GetIterator();
	while (it.Next())
	{
		OperatorProperty* property = it.Current();
		if (property->IsLinked())
		{
			property->GetSlot()->GetOwner()->Refresh();
		}
	}

	DoRefresh();

	_Valid = true;
}

void Operator::Create()
{
	const TypeInfo* type = GetType();
	if (type == NULL)
		return;

	const FieldList& fields = type->GetFields();
	const FieldInfo* field;
	foreach (field, fields, FieldList)
	{
		if (field->GetName() == "_ID" || field->GetName() == "_name")
			continue;

		OperatorProperty* property = new OperatorProperty(this, field);
		property->SetName(field->GetName());
		property->SetCanLink(true);
		_Properties.Add(property);
	}
}

void Operator::Destroy()
{
}

void Operator::Update()
{
}

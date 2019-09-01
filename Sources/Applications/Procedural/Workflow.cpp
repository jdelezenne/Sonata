/*=============================================================================
Workflow.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Workflow.h"
#include "Procedural.h"

SE_IMPLEMENT_CLASS(Workflow);
SE_IMPLEMENT_REFLECTION(Workflow);

Workflow::Workflow() :
	RefObject()
{
	_SelectedOperator = NULL;

	_IDs = 1;
	_Width = AppCore::Instance()->Settings()->_PreviewSize.Width;
	_Height = AppCore::Instance()->Settings()->_PreviewSize.Height;

	 _Diagram = new UI::Diagram();
	 _s = _t = 0.0f;
}

Workflow::~Workflow()
{
	OperatorList::Iterator it = _Operators.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}

	//SE_DELETE(_Diagram);
}

void Workflow::OnSerialized(BinarySerializer* context, BinaryStream& stream)
{
	super::OnSerialized(context, stream);

	int count = _Operators.Count();
	stream.WriteInt32(count);
	for (int i=0; i<count; i++)
	{
		context->Serialize(stream, _Operators.GetItem(i), _Operators.GetItem(i)->GetType());
	}
}

void Workflow::OnSerialized(XMLSerializer* context, XMLElement* element)
{
	super::OnSerialized(context, element);

	SonataEngine::XMLDocument* document = element->GetOwnerDocument();
	XMLElement* array = document->CreateElement("Operators");
	int count = _Operators.Count();
	array->SetAttribute("Count", String::ToString(count));
	for (int i=0; i<count; i++)
	{
		XMLElement* item = document->CreateElement("Operator");
		context->Serialize(document, item, _Operators.GetItem(i), _Operators.GetItem(i)->GetType());
		array->AppendChild(item);
	}
	element->AppendChild(array);
}

void Workflow::OnDeserialized(BinarySerializer* context, BinaryStream& stream)
{
	super::OnDeserialized(context, stream);

	int count = stream.ReadInt32();
	for (int i=0; i<count; i++)
	{
		Operator* op = (Operator*)context->Deserialize(stream);
		if (op != NULL)
			_Operators.Add(op);
	}
}

void Workflow::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	super::OnDeserialized(context, element);

	XMLElement* array = (XMLElement*)element->GetItem("Operators");
	int count = array->GetAttribute("Count").ToInt32();
	XMLElement* item = (XMLElement*)array->GetItem("Operator");
	for (int i=0; i<count; i++)
	{
		XMLElement* type = (XMLElement*)item->GetFirstChild();
		Operator* op = (Operator*)context->Deserialize(item->GetOwnerDocument(), type);
		if (op != NULL)
			_Operators.Add(op);
		item = (XMLElement*)item->GetNextSibling();
	}

	// Create the nodes
	{
		OperatorList::Iterator it = _Operators.GetIterator();
		while (it.Next())
		{
			Operator* op = it.Current();
			op->SetWorkflow(this);

			UI::TableNode* table = op->CreateNode();
			table->SetPosition(op->_Position);
		}
	}

	// Link the operators
	{
		OperatorList::Iterator it = _Operators.GetIterator();
		while (it.Next())
		{
			Operator* op = it.Current();

			int count = op->_SlotRefs.Count();
			for (int i=0; i<count; i++)
			{
				const Operator::SlotRef& slotRef = op->_SlotRefs[i];
				OperatorProperty* property = op->_Properties[slotRef._Index];

				Operator* source = GetOperatorByID(slotRef._ID);
				if (source != NULL)
				{
					OperatorSlot* slot = source->Slot(slotRef._name);
					property->LinkTo(slot);

					UI::TableProperty* srcProperty =
						source->_Node->OutputProperties().GetItem(
						source->Slots().IndexOf(slot));
					UI::TableProperty* destProperty =
						op->_Node->InputProperties().GetItem(slotRef._Index);

					_Diagram->CreateLink(srcProperty, srcProperty->Slots().GetItem(0),
						destProperty, destProperty->Slots().GetItem(0));
				}
			}
		}
	}
}

#define EITHER_ARE(tA, tB) \
	((t0 == typeof(tA) && t1 == typeof(tB)) || (t0 == typeof(tB) && t1 == typeof(tA)))

bool Workflow::IsTypeCompatible(const TypeInfo* t0, const TypeInfo* t1)
{
	// Same type
	if (t0 == t1)
		return true;

	// Two primitive types are compatible
	if (t0->IsPrimitive() && t1->IsPrimitive())
		return true;

	// A primitive type is not compatible with a non-primitive type
	//if ((t0->IsPrimitive() && !t1->IsPrimitive()) || (!t0->IsPrimitive() && t1->IsPrimitive()))
	//	return false;

	if (EITHER_ARE(Color8, Vector3))
		return true;

	return false;
}

void Workflow::InitializeOperators()
{
	OperatorList::Iterator it = _Operators.GetIterator();
	while (it.Next())
	{
		Operator* op = (Operator*)it.Current();
		AppCore::Instance()->GetMainUI()->RegisterTableEvents(op->_Node);
	}
}

void Workflow::AddOperator(Operator* op)
{
	_Operators.Add(op);
	op->SetID(_IDs);
	_IDs++;
}

void Workflow::RemoveOperator(Operator* op)
{
	if (op == NULL || !_Operators.Contains(op))
		return;

	if (!_RemovedOperators.Contains(op))
		_RemovedOperators.Add(op);
}

Operator* Workflow::GetOperatorByID(int32 id) const
{
	OperatorList::Iterator it = _Operators.GetIterator();
	while (it.Next())
	{
		Operator* op = (Operator*)it.Current();
		if (op->GetID() == id)
			return op;
	}

	return NULL;
}

Operator* Workflow::GetSelectedOperator() const
{
	return _SelectedOperator;
}

void Workflow::SetSelectedOperator(Operator* value)
{
	if (value != NULL && !_Operators.Contains(value))
		return;

	const UI::NodeList& nodes = _Diagram->Nodes();
	UI::NodeList::Iterator it = nodes.GetIterator();
	while (it.Next())
	{
		UI::Node* cur = (UI::Node*)it.Current();
		cur->SetSelected(false);
	}

	if (value != NULL)
	{
		value->_Node->SetSelected(true);
		UI::UISystem::Instance()->SetFocusedWidget(value->_Node);
	}

	_SelectedOperator = value;
}

void Workflow::Update()
{
	// Unlink the nodes connected to the node to be removed
	if (!_RemovedOperators.IsEmpty())
	{
		OperatorList::Iterator itC = _Operators.GetIterator();
		while (itC.Next())
		{
			Operator* op = itC.Current();
			if (_RemovedOperators.Contains(op))
				continue;

			const OperatorPropertyList& properties = op->Properties();

			// For each property
			OperatorPropertyList::Iterator itP = properties.GetIterator();
			while (itP.Next())
			{
				OperatorProperty* property = itP.Current();

				// Check if the property is linked
				if (property->IsLinked())
				{
					// If it is, check if it is linked to a deleted operator
					if (_RemovedOperators.Contains(property->GetSlot()->GetOwner()))
					{
						// Unlink it then
						property->Unlink();
					}
				}
			}

			const OperatorSlotList& slots = op->Slots();

			OperatorSlotList::Iterator itS = slots.GetIterator();
			while (itS.Next())
			{
				OperatorSlot* slot = itS.Current();
			}
		}
	}

	if (_RemovedOperators.Contains(_SelectedOperator))
	{
		_SelectedOperator = NULL;
		AppCore::Instance()->GetMainUI()->SetSelectedOperator(NULL);
	}

	// Remove the nodes
	OperatorList::Iterator it = _RemovedOperators.GetIterator();
	while (it.Next())
	{
		_Operators.Remove(it.Current());
		delete it.Current();
	}

	_RemovedOperators.Clear();

	Refresh();
}

void Workflow::Refresh()
{
	// Invalidate the operators linked to the slots of invalid operators
	{
		// Iterate the operators
		OperatorList::Iterator itC1 = _Operators.GetIterator();
		while (itC1.Next())
		{
			Operator* operator1 = itC1.Current();

			// The operator is valid, skip it
			if (operator1->IsValid())
				continue;

			// Iterate the slots of this operator
			OperatorSlotList::Iterator itS = operator1->_Slots.GetIterator();
			while (itS.Next())
			{
				OperatorSlot* slot = itS.Current();

				// Iterate the operators to find the properties linked to this slot
				OperatorList::Iterator itC2 = _Operators.GetIterator();
				while (itC2.Next())
				{
					Operator* operator2 = itC2.Current();

					// The operator is already invalid, skip it
					if (!operator2->IsValid())
						continue;

					// Iterate the properties of this operator
					OperatorPropertyList::Iterator itP = operator2->_Properties.GetIterator();
					while (itP.Next())
					{
						OperatorProperty* property = itP.Current();
						if (property->IsLinked())
						{
							// Invalidate the owner
							operator2->Invalidate();
							break; // Don't check the other properties
						}
					}
				}
			}
		}
	}

	// Refresh the invalidated operators
	OperatorList::Iterator it = _Operators.GetIterator();
	while (it.Next())
	{
		Operator* op = it.Current();
		op->Refresh();
	}
}


SE_IMPLEMENT_CLASS(OperatorLibrary);

OperatorLibrary::OperatorLibrary() :
	Object()
{
}

OperatorLibrary::~OperatorLibrary()
{
}

void OperatorLibrary::OnDeserialized(XMLSerializer* context, XMLElement* element)
{
	Object::OnDeserialized(context, element);

	XMLElement* array = (XMLElement*)element->GetItem("Categories");
	int count = array->GetAttribute("Count").ToInt32();
	XMLElement* item = (XMLElement*)array->GetItem("Category");
	for (int i=0; i<count; i++)
	{
		OperatorCategory category = DeserializeCategory(context, item);
		_Categories.Add(category);
		item = (XMLElement*)item->GetNextSibling();
	}
}

OperatorCategory OperatorLibrary::DeserializeCategory(XMLSerializer* context, XMLElement* element)
{
	OperatorCategory category;
	XMLElement* elementName = (XMLElement*)element->GetItem("Name");
	if (elementName != NULL)
	{
		XMLText* valueName = (XMLText*)elementName->GetFirstChild();
		if (valueName != NULL)
			category.Name = valueName->GetValue();
	}

	XMLElement* elementDesc = (XMLElement*)element->GetItem("Description");
	if (elementDesc != NULL)
	{
		XMLText* valueDesc = (XMLText*)elementDesc->GetFirstChild();
		if (valueDesc != NULL)
			category.Description = valueDesc->GetValue();
	}

	XMLElement* array = (XMLElement*)element->GetItem("Operators");
	int count = array->GetAttribute("Count").ToInt32();
	XMLElement* item = (XMLElement*)array->GetItem("Operator");
	for (int i=0; i<count; i++)
	{
		OperatorDescription description = DeserializeDescription(context, item);
		category.Operators.Add(description);
		item = (XMLElement*)item->GetNextSibling();
	}

	return category;
}

OperatorDescription OperatorLibrary::DeserializeDescription(XMLSerializer* context, XMLElement* element)
{
	OperatorDescription operatorDesc;
	XMLElement* elementName = (XMLElement*)element->GetItem("Name");
	if (elementName != NULL)
	{
		XMLText* valueName = (XMLText*)elementName->GetFirstChild();
		if (valueName != NULL)
			operatorDesc.Name = valueName->GetValue();
	}

	XMLElement* elementDesc = (XMLElement*)element->GetItem("Description");
	if (elementDesc != NULL)
	{
		XMLText* valueDesc = (XMLText*)elementDesc->GetFirstChild();
		if (valueDesc != NULL)
			operatorDesc.Description = valueDesc->GetValue();
	}

	XMLElement* elementTypeName = (XMLElement*)element->GetItem("TypeName");
	if (elementTypeName != NULL)
	{
		XMLText* valueTypeName = (XMLText*)elementTypeName->GetFirstChild();
		if (valueTypeName != NULL)
			operatorDesc.TypeName = valueTypeName->GetValue();
	}

	return operatorDesc;
}

OperatorLibrary::CategoryList OperatorLibrary::Categories() const
{
	return _Categories;
}

const OperatorCategory* OperatorLibrary::GetCategoryByName(const String& name) const
{
	int count = _Categories.Count();
	for (int i=0; i<count; i++)
	{
		if (_Categories[i].Name == name)
			return &_Categories[i];
	}

	return NULL;
}

Operator* OperatorLibrary::CreateOperator(const String& name)
{
	return (Operator*)TypeFactory::Instance()->CreateInstance(name + "Operator");
}

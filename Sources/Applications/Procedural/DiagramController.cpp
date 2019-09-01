/*=============================================================================
DiagramController.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DiagramController.h"
#include "Procedural.h"
#include "Workflow.h"

bool IsLinkValid(UI::TableProperty* source, UI::TableProperty* destination)
{
	if (source->GetPropertyType() == destination->GetPropertyType())
		return false;

	if (source->GetOwner() == destination->GetOwner())
		return false;

	OperatorSlot* srcSlot = (OperatorSlot*)source->GetUserData().ToObject();
	OperatorProperty* destProperty = (OperatorProperty*)destination->GetUserData().ToObject();
	if (!Workflow::IsTypeCompatible(srcSlot->GetType(), destProperty->GetType()))
		return false;

	Operator* srcOperator = (Operator*)source->GetOwner()->GetUserData().ToObject();
	Operator* destOperator = (Operator*)destination->GetOwner()->GetUserData().ToObject();
	if (srcOperator->HasSource(destOperator))
		return false;

	return true;
}

DiagramController::DiagramController(UI::Diagram* diagram)
{
	_Diagram = diagram;
	_LinkedProperty = NULL;
	_Linking = false;
	_Link = NULL;
}

bool DiagramController::OnDiagramMouseMove(Object* sender, const UI::MouseEventArgs& e)
{
	if (_Linking)
	{
		_Link->SetLineColor(Color32::Black);
	}

	return true;
}

bool DiagramController::OnNodeMouseDown(Object* sender, const UI::MouseEventArgs& e)
{
	UI::Node* node = (UI::Node*)sender;
	Operator* op = (Operator*)node->GetUserData().ToObject();

	AppCore::Instance()->GetWorkflow()->SetSelectedOperator(op);

	return true;
}

bool DiagramController::OnNodeKeyDown(Object* sender, const UI::KeyEventArgs& e)
{
	UI::TableNode* node = (UI::TableNode*)sender;

	if (e.Key() == Input::Key_Delete)
	{
		Operator* op = (Operator*)node->GetUserData().ToObject();

		// Invalidate the operators linked to the slots of this node
		const UI::TableNode::TablePropertyList& properties = node->OutputProperties();
		UI::TableNode::TablePropertyList::Iterator itP = properties.GetIterator();
		while (itP.Next())
		{
			UI::TableProperty* tableProperty = itP.Current();
			UI::ConnectionList connections = tableProperty->GetConnectionsOnSlot(
				tableProperty->Slots().GetItem(0));

			UI::ConnectionList::Iterator it = connections.GetIterator();
			while (it.Next())
			{
				UI::Connection* connection = it.Current();
				UI::Slot* slot = connection->GetDestinationSlot();
				UI::TableProperty* destination = (UI::TableProperty*)slot->GetParent();
				OperatorProperty* operatorProperty = (OperatorProperty*)destination->GetUserData().ToObject();
				operatorProperty->Unlink();
				operatorProperty->GetOwner()->Invalidate();
			}
		}

		// Remove the operator
		op->GetWorkflow()->RemoveOperator(op);
	}

	return true;
}

bool DiagramController::OnTablePostRender(Object* sender, const UI::DrawEventArgs& e)
{
	UI::TableProperty* property = (UI::TableProperty*)sender;

	if (_LinkedProperty == property && _Linking)
	{
		_Link->DrawLink(property->Slots().GetItem(0)->GetPosition(), _Destination);
	}

	return true;
}

bool DiagramController::OnTableMouseMove(Object* sender, const UI::MouseEventArgs& e)
{
	if (!_Linking)
	{
		return true;
	}

	_Destination = e.Location();

	UI::Widget* widget = _Diagram->GetChildAtPoint(e.Location());
	if (widget == NULL || widget->GetType() != UI::TableProperty::GetTypeStatic())
	{
		return true;
	}

	UI::TableProperty* destination = (UI::TableProperty*)widget;

	bool validLink = IsLinkValid(_LinkedProperty, destination);

	if (!validLink)
	{
		_Link->SetLineColor(Color32::Red);
	}
	else
	{
		_Link->SetLineColor(Color32::Black);
	}

	return true;
}

bool DiagramController::OnTableMouseDown(Object* sender, const UI::MouseEventArgs& e)
{
	UI::TableProperty* property = (UI::TableProperty*)sender;

	if (e.Button() == UI::MouseButtons_Left)
	{
		OperatorProperty* operatorProperty = (OperatorProperty*)property->GetUserData().ToObject();
		AppCore::Instance()->GetWorkflow()->SetSelectedOperator(operatorProperty->GetOwner());

		if (property->GetPropertyType() == UI::TablePropertyType_Output)
		{
			if (operatorProperty->GetCanLink())
			{
				_Linking = true;
				UI::UISystem::Instance()->SetCapture(property);

				_Link = new UI::Link();
				_Link->SetDiagram(_Diagram);
				_Destination = e.Location();

				_LinkedProperty = property;
			}
		}
	}

	return true;
}

bool DiagramController::OnTableMouseUp(Object* sender, const UI::MouseEventArgs& e)
{
	UI::TableProperty* source = (UI::TableProperty*)sender;

	if (e.Button() == UI::MouseButtons_Left)
	{
		if (!_Linking)
		{
			return true;
		}

		_Linking = false;
		UI::UISystem::Instance()->ReleaseCapture();

		_Diagram->RemoveNode(_Link);
		_Link = NULL;

		_LinkedProperty = NULL;

		UI::Widget* widget = _Diagram->GetChildAtPoint(e.Location());
		if (widget == NULL || widget->GetType() != UI::TableProperty::GetTypeStatic())
		{
			return true;
		}

		UI::TableProperty* destination = (UI::TableProperty*)widget;
		bool validLink = IsLinkValid(source, destination);

		if (!validLink)
		{
			return true;
		}

		OperatorSlot* operatorSlot = (OperatorSlot*)source->GetUserData().ToObject();
		OperatorProperty* operatorProperty = (OperatorProperty*)destination->GetUserData().ToObject();

		if (operatorProperty->GetSlot() == operatorSlot)
		{
			return true;
		}

		// Unlink the input
		_Diagram->DestroyLink(destination, destination->Slots().GetItem(0));

		if (destination->GetPropertyType() == UI::TablePropertyType_Input)
		{
			OperatorProperty* property = (OperatorProperty*)destination->GetUserData().ToObject();
			property->Unlink();
			property->GetOwner()->Invalidate();
		}

		// Link the nodes
		_Diagram->CreateLink(source, source->Slots().GetItem(0),
			destination, destination->Slots().GetItem(0));

		// Link the operators
		operatorProperty->LinkTo(operatorSlot);
		operatorSlot->GetOwner()->Invalidate();
		operatorProperty->GetOwner()->Invalidate();
	}
	else if (e.Button() == UI::MouseButtons_Right)
	{
		if (source->GetPropertyType() == UI::TablePropertyType_Input)
		{
			OperatorProperty* operatorProperty = (OperatorProperty*)source->GetUserData().ToObject();
			operatorProperty->Unlink();
			operatorProperty->GetOwner()->Invalidate();
		}
		else
		{
			UI::ConnectionList connections = source->GetConnectionsOnSlot(
				source->Slots().GetItem(0));

			UI::ConnectionList::Iterator it = connections.GetIterator();
			while (it.Next())
			{
				UI::Connection* connection = it.Current();
				UI::Slot* slot = connection->GetDestinationSlot();
				UI::TableProperty* destination = (UI::TableProperty*)slot->GetParent();
				OperatorProperty* operatorProperty = (OperatorProperty*)destination->GetUserData().ToObject();
				operatorProperty->Unlink();
				operatorProperty->GetOwner()->Invalidate();
			}
		}

		_Diagram->DestroyLink(source, source->Slots().GetItem(0));
	}

	return true;
}

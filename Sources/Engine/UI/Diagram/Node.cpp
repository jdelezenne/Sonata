/*=============================================================================
Node.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Node.h"
#include "UI/UISystem.h"
#include "UI/Diagram/Connection.h"
#include "UI/Diagram/Diagram.h"
#include "UI/Diagram/Slot.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Node);
		SE_IMPLEMENT_REFLECTION(Node);

		Node::Node() :
			super()
		{
			_Sizable = false;

			_Diagram = NULL;
			_Selected = false;

			_SelectedColor = Color32::Black;
		}

		Node::~Node()
		{
			SlotList::Iterator it = _Slots.GetIterator();
			while (it.Next())
			{
				Slot* slot = it.Current();
				_Diagram->DestroyLink(this, slot);
			}
		}

		void Node::AddNode(Node* node)
		{
			Children().Add(node);
			node->SetDiagram(_Diagram);
		}

		void Node::RemoveNode(Node* node)
		{
			if (Children().Contains(node))
			{
				Children().Remove(node);
			}
		}

		Connection* Node::FindConnection(Slot* source, Slot* destination)
		{
			ConnectionList::Iterator it = _Connections.GetIterator();
			while (it.Next())
			{
				Connection* connection = it.Current();
				if (connection->GetSourceSlot() == source && connection->GetDestinationSlot() == destination)
				{
					return connection;
				}
			}

			return NULL;
		}

		ConnectionList Node::GetConnectionsOnSlot(Slot* slot)
		{
			ConnectionList connections;
			ConnectionList::Iterator it = _Connections.GetIterator();
			while (it.Next())
			{
				Connection* connection = it.Current();
				if (connection->GetSourceSlot() == slot || connection->GetDestinationSlot() == slot)
				{
					connections.Add(connection);
				}
			}

			return connections;
		}

		Connection* Node::Connect(Slot* source, Slot* destination)
		{
			if (source == NULL || destination == NULL)
				return NULL;

			Connection* connection = FindConnection(source, destination);

			if (connection == NULL)// && Slots().Contains(source))
			{
				connection = new Connection();
				connection->SetSourceSlot(source);
				connection->SetDestinationSlot(destination);
				_Connections.Add(connection);
			}

			return connection;
		}

		void Node::Disconnect(Slot* source, Slot* destination)
		{
			Connection* connection = FindConnection(source, destination);
			if (connection != NULL)
			{
				_Connections.Remove(connection);

				if (source->GetParent() == this)
				{
					((Node*)destination->GetParent())->Disconnect(
						destination, source);
				}
				else if (destination->GetParent() == this)
				{
					((Node*)source->GetParent())->Disconnect(
						source, destination);
				}
			}
		}

		void Node::DisconnectAll(Slot* slot)
		{
			ConnectionList::Iterator it = _Connections.GetIterator();
			while (it.Next())
			{
				Connection* connection = it.Current();

				if (connection->GetSourceSlot() == slot || connection->GetDestinationSlot() == slot)
				{
					_Connections.Remove(connection);
					it = _Connections.GetIterator();
				}

				if (connection->GetSourceSlot() == slot)
				{
					((Node*)connection->GetDestinationSlot()->GetParent())->Disconnect(
						connection->GetDestinationSlot(), slot);
				}
				else if (connection->GetDestinationSlot() == slot)
				{
					((Node*)connection->GetSourceSlot()->GetParent())->Disconnect(
						slot, connection->GetSourceSlot());
				}
			}
		}

		void Node::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);

			SlotList::Iterator itS = _Slots.GetIterator();
			while (itS.Next())
			{
				Slot* slot = itS.Current();
				slot->Update(elapsed);
			}
		}

		void Node::DoRender()
		{
			Color32 color = GetFrameColor();
			if (GetSelected())
			{
				color = GetSelectedColor();
			}

			if (GetHasFrame())
			{
				RectangleInt frameRect = LocalToGlobal(RectangleInt(PointInt(0, 0), GetSize()));
				UISystem::Instance()->DrawRectangle(color, GetFrameSize(), frameRect);
			}

			if (GetHasBackground())
			{
				RectangleInt bgRect = LocalToGlobal(GetClientRectangle());
				UISystem::Instance()->FillRectangle(GetBackgroundColor(), bgRect);
			}

			/*SlotList::Iterator itS = _Slots.GetIterator();
			while (itS.Next())
			{
				Slot* slot = itS.Current();
				slot->Render();
			}*/
		}
	}
}

/*=============================================================================
Diagram.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Diagram.h"
#include "UI/UISystem.h"
#include "UI/Diagram/Connection.h"
#include "UI/Diagram/Link.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Diagram);
		SE_IMPLEMENT_REFLECTION(Diagram);

		Diagram::Diagram() :
			super()
		{
		}

		Diagram::~Diagram()
		{
		}

		void Diagram::AddNode(Node* node)
		{
			Children().Add(node);
			node->SetDiagram(this);
		}

		void Diagram::RemoveNode(Node* node)
		{
			if (Children().Contains(node))
			{
				const SlotList& slots = node->Slots();
				SlotList::Iterator it = slots.GetIterator();
				while (it.Next())
				{
					Slot* slot = it.Current();
					DestroyLink(node, slot);
				}

				Children().Remove(node);
				UISystem::Instance()->DestroyWidget(node);
			}
		}

		void Diagram::CreateLink(Node* srcNode, Slot* srcSlot, Node* destNode, Slot* destSlot)
		{
			if (srcNode != destNode &&
				srcNode->Slots().Contains(srcSlot) && destNode->Slots().Contains(destSlot))
			{
				// Create the connections
				srcNode->Connect(srcSlot, destSlot);
				destNode->Connect(destSlot, srcSlot);

				// Create the link
				Link* link = new Link();
				link->SetStartSlot(srcSlot);
				link->SetEndSlot(destSlot);
				AddNode(link);
			}
		}

		void Diagram::DestroyLink(Node* node, Slot* slot)
		{
			// Remove the connections
			node->DisconnectAll(slot);

			// Remove the links
			NodeList::Iterator it = _Children->GetIterator();
			while (it.Next())
			{
				Node* child = (Node*)it.Current();
				if (child->GetType() == Link::GetTypeStatic())
				{
					Link* link = (Link*)child;
					if (link->GetStartSlot() == slot || link->GetEndSlot() == slot)
					{
						Children().Remove(link);
						it = _Children->GetIterator();
					}
				}
			}
		}

		void Diagram::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void Diagram::DoRender()
		{
			super::DoRender();
		}
	}
}

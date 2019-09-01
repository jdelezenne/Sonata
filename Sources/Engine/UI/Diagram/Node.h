/*=============================================================================
Node.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIAGRAM_NODE_H_
#define _SE_UI_DIAGRAM_NODE_H_

#include "UI/Common.h"
#include "UI/Panel.h"

namespace SonataEngine
{
	namespace UI
	{
		class Connection;
		class Diagram;
		class Slot;

		typedef Widget::WidgetList NodeList;
		typedef Array<Connection*> ConnectionList;
		typedef Array<Slot*> SlotList;

		/*class SE_UI_EXPORT INode
		{
		public:
			virtual void SetParent(INode* value) const = 0;
			virtual NodeList& Nodes() = 0;
		};*/

		/**
			@brief Base class for the diagram nodes.
		*/
		class SE_UI_EXPORT Node : public Panel//, public INode
		{
			SE_DECLARE_CLASS(Node, Panel);

			SE_BEGIN_REFLECTION(Node);
			SE_END_REFLECTION(Node);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Node();

			/** Destructor. */
			virtual ~Node();
			//@}

			/** @name Properties. */
			//@{
			Diagram* GetDiagram() const { return _Diagram; }
			void SetDiagram(Diagram* value) { _Diagram = value; }

			//virtual void SetParent(INode* value) const { _Parent = value; }

			/** List of child nodes belonging to the node. */
			NodeList& Nodes() { return Children(); }
			void AddNode(Node* node);
			void RemoveNode(Node* node);
			//virtual NodeList& Nodes() { return _Nodes; }

			/** List of slots belonging to the node. */
			SlotList& Slots() { return _Slots; }

			/** List of connections made with this node. */
			ConnectionList& Connections() { return _Connections; }

			bool GetSelected() const { return _Selected; }
			void SetSelected(bool value) { _Selected = value; }

			Color32 GetSelectedColor() const { return _SelectedColor; }
			void SetSelectedColor(const Color32& value) { _SelectedColor = value; }
			//@}

			/** @name Operations. */
			//@{
			Connection* FindConnection(Slot* source, Slot* destination);
			ConnectionList GetConnectionsOnSlot(Slot* slot);

			/** Create a new connection between the given slots. */
			Connection* Connect(Slot* source, Slot* destination);

			/** Removes the connection containing the specified slots. */
			void Disconnect(Slot* source, Slot* destination);
			void DisconnectAll(Slot* slot);

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			//INode* _Parent;
			//NodeList _Nodes;
			Diagram* _Diagram;
			SlotList _Slots;
			ConnectionList _Connections;
			bool _Selected;

			Color32 _SelectedColor;
		};
	}
}

#endif 

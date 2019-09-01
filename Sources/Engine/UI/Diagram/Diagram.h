/*=============================================================================
Diagram.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIAGRAM_DIAGRAM_H_
#define _SE_UI_DIAGRAM_DIAGRAM_H_

#include "UI/Common.h"
#include "UI/ScrollablePanel.h"
#include "UI/Diagram/Node.h"

namespace SonataEngine
{
	namespace UI
	{
		class SE_UI_EXPORT NodeEventArgs : public EventArgs
		{
		public:
			NodeEventArgs(Node* node) :
				EventArgs(),
				_Node(node)
			{
			}

			Node* Node() const { return _Node; }

		protected:
			UI::Node* _Node;
		};

		class SE_UI_EXPORT ConnectionEventArgs : public EventArgs
		{
		public:
			ConnectionEventArgs(Connection* connection) :
				EventArgs(),
				_Connection(connection)
			{
			}

			Connection* Connection() const { return _Connection; }

		protected:
			UI::Connection* _Connection;
		};

		/**
			@brief Widget for diagramming.
		*/
		class SE_UI_EXPORT Diagram : public ScrollablePanel//, public INode
		{
			SE_DECLARE_CLASS(Diagram, ScrollablePanel);

			SE_BEGIN_REFLECTION(Diagram);
			SE_END_REFLECTION(Diagram);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Diagram();

			/** Destructor. */
			virtual ~Diagram();
			//@}

			/** @name Properties. */
			//@{
			/** List of child nodes belonging to the diagram. */
			NodeList& Nodes() { return Children(); }
			void AddNode(Node* node);
			void RemoveNode(Node* node);
			//NodeList& Nodes() { return _Nodes; }
			//@}

			/** @name Operations. */
			//@{
			void CreateLink(Node* srcNode, Slot* srcSlot, Node* destNode, Slot* destSlot);
			void DestroyLink(Node* node, Slot* slot);

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			//NodeList _Nodes;
		};
	}
}

#endif 

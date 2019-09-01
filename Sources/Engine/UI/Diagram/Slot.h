/*=============================================================================
Slot.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIAGRAM_SLOT_H_
#define _SE_UI_DIAGRAM_SLOT_H_

#include "UI/Common.h"
#include "UI/Diagram/Node.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Slot belonging to a node.
		*/
		class SE_UI_EXPORT Slot : public Node
		{
			SE_DECLARE_CLASS(Slot, Node);

			SE_BEGIN_REFLECTION(Slot);
			SE_END_REFLECTION(Slot);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Slot();

			/** Destructor. */
			virtual ~Slot();
			//@}

			/** @name Properties. */
			//@{
			Color32 GetSlotColor() const { return _SlotColor; }
			void SetSlotColor(Color32 value) { _SlotColor = value; }
			//@}

			/** @name Operations. */
			//@{
			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			Color32 _SlotColor;
		};
	}
}

#endif 

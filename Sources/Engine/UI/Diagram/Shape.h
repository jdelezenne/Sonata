/*=============================================================================
Shape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIAGRAM_SHAPE_H_
#define _SE_UI_DIAGRAM_SHAPE_H_

#include "UI/Common.h"
#include "UI/Diagram/Node.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Node representing a drawing primitive.
		*/
		class SE_UI_EXPORT Shape : public Node
		{
			SE_DECLARE_CLASS(Shape, Node);

			SE_BEGIN_REFLECTION(Shape);
			SE_END_REFLECTION(Shape);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Shape();

			/** Destructor. */
			virtual ~Shape();
			//@}

			/** @name Properties. */
			//@{
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
		};
	}
}

#endif 

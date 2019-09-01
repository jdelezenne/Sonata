/*=============================================================================
Link.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIAGRAM_LINK_H_
#define _SE_UI_DIAGRAM_LINK_H_

#include "UI/Common.h"
#include "UI/Diagram/Node.h"

namespace SonataEngine
{
	namespace UI
	{
		enum LinkShape
		{
			LinkShape_Line,
			LinkShape_OrthogonalLine
		};

		/**
			@brief Represents a connection.
		*/
		class SE_UI_EXPORT Link : public Node
		{
			SE_DECLARE_CLASS(Link, Node);

			SE_BEGIN_REFLECTION(Link);
			SE_END_REFLECTION(Link);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Link();

			/** Destructor. */
			virtual ~Link();
			//@}

			/** @name Properties. */
			//@{
			LinkShape GetLinkShape() const { return _LinkShape; }
			void SetLinkShape(LinkShape value) { _LinkShape = value; }

			Color32 GetLineColor() const { return _LineColor; }
			void SetLineColor(Color32 value) { _LineColor = value; }

			int32 GetLineSize() const { return _LineSize; }
			void SetLineSize(int32 value) { _LineSize = value; }
			//@}

			/** @name Operations. */
			//@{
			Slot* GetStartSlot() const;
			void SetStartSlot(Slot* value);

			Slot* GetEndSlot() const;
			void SetEndSlot(Slot* value);

			virtual void DrawLink(const PointInt& source, const PointInt& destination);

			/** Perform the update of the widget. */
			virtual void DoUpdate(real64 elapsed);

			/** Perform the rendering of the widget. */
			virtual void DoRender();
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			LinkShape _LinkShape;

			Color32 _LineColor;
			int32 _LineSize;
		};
	}
}

#endif 

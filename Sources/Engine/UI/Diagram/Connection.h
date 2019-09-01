/*=============================================================================
Connection.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_DIAGRAM_CONNECTION_H_
#define _SE_UI_DIAGRAM_CONNECTION_H_

#include "UI/Common.h"

namespace SonataEngine
{
	namespace UI
	{
		class Slot;

		/**
			@brief Binds two slots together.
		*/
		class SE_UI_EXPORT Connection : public Object
		{
			SE_DECLARE_CLASS(Connection, Object);

			SE_BEGIN_REFLECTION(Connection);
			SE_END_REFLECTION(Connection);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Connection();

			/** Destructor. */
			virtual ~Connection();
			//@}

			/** @name Properties. */
			//@{
			Slot* GetSourceSlot() const { return _SourceSlot; }
			void SetSourceSlot(Slot* value) { _SourceSlot = value; }

			Slot* GetDestinationSlot() const { return _DestinationSlot; }
			void SetDestinationSlot(Slot* value) { _DestinationSlot = value; }
			//@}

			/** @name Operations. */
			//@{
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			Slot* _SourceSlot;
			Slot* _DestinationSlot;
		};
	}
}

#endif 

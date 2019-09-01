/*=============================================================================
ContainerPanel.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_CONTAINERPANEL_H_
#define _SE_UI_CONTAINERPANEL_H_

#include "UI/Common.h"
#include "UI/ScrollablePanel.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Base class for the panels that can function as a container for other widgets.
		*/
		class SE_UI_EXPORT ContainerPanel : public ScrollablePanel
		{
			SE_DECLARE_CLASS(ContainerPanel, ScrollablePanel);

			SE_BEGIN_REFLECTION(ContainerPanel);
			SE_END_REFLECTION(ContainerPanel);

		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			ContainerPanel();

			/** Destructor. */
			virtual ~ContainerPanel();
			//@}

			/** @name Properties. */
			//@{
			//@}

			/** @name Operations. */
			//@{
			/** Adjusts the scroll bars on the container based on the current widgets positions. */
			void UpdateScrollbars();

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

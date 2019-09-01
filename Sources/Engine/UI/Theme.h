/*=============================================================================
Theme.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_UI_THEME_H_
#define _SE_UI_THEME_H_

#include "UI/Common.h"

namespace SonataEngine
{
	namespace UI
	{
		/**
			@brief Provides the look and feel of the user interface.

			@todo Look at "ASP.NET Themes and Skins".
		*/
		class SE_UI_EXPORT Theme : public Object
		{
		public:
			/** @name Constructor / Destructor. */
			//@{
			/** Constructor. */
			Theme();

			/** Destructor. */
			virtual ~Theme();
			//@}

			/** @name Properties. */
			//@{
			String GetName() const { return _name; }
			void SetName(const String& value) { _name = value; }
			//@}

			/** @name Operations. */
			//@{
			//@}

			/** @name Events. */
			//@{
			//@}

		protected:
			String _name;
		};
	}
}

#endif 

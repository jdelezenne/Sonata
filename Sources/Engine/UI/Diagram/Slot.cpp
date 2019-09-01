/*=============================================================================
Slot.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Slot.h"
#include "UI/UISystem.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Slot);
		SE_IMPLEMENT_REFLECTION(Slot);

		Slot::Slot() :
			super()
		{
		}

		Slot::~Slot()
		{
		}

		void Slot::DoUpdate(real64 elapsed)
		{
			super::DoUpdate(elapsed);
		}

		void Slot::DoRender()
		{
			super::DoRender();
		}
	}
}

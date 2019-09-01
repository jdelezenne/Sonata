/*=============================================================================
Connection.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Connection.h"
#include "UI/UISystem.h"
#include "UI/Diagram/Slot.h"

namespace SonataEngine
{
	namespace UI
	{
		SE_IMPLEMENT_CLASS(Connection);
		SE_IMPLEMENT_REFLECTION(Connection);

		Connection::Connection() :
			super()
		{
			_SourceSlot = NULL;
			_DestinationSlot = NULL;
		}

		Connection::~Connection()
		{
		}

	}
}

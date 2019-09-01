/*=============================================================================
Message.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Message.h"

namespace SampleFramework
{
    MessageDesc::MessageDesc(const String& name, EntityID id) :
		_name(name),
		_ID(id)
	{
	}

	MessageDesc::~MessageDesc()
	{
	}

	bool MessageDesc::operator==(const MessageDesc& value) const
	{
		return _ID == value.GetID();
	}

	uint32 MessageDesc::GenerateIDFromString(const String& str)
	{
		const SEchar* ptr = str.Data();
		uint32 id = 0;

		while (*ptr != 0)
		{
			id = (id<<1) ^ (static_cast<uint32>(*ptr));
			ptr++;
		}

		return id;
	}

	Message::Message(MessageDesc& desc) :
		_Desc(desc)
	{
	}

	Message::~Message()
	{
	}
}

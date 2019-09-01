/*=============================================================================
Message.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_MESSAGE_H_
#define _SFW_MESSAGE_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{
	/**
		@brief Message description.

		A message is defined by its description. A MessageDesc represents
		the type of message and a Message represents an instance of a message.
	*/
	class SE_SAMPLEFRAMEWORK_EXPORT MessageDesc
	{
	public:
		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		MessageDesc(const String& name, MessageID id);

		/** Destructor. */
		virtual ~MessageDesc();
		//@}

		bool operator==(const MessageDesc& value) const;

		String GetName() const { return _name; }

		MessageID GetID() const { return _ID; }

		static uint32 GenerateIDFromString(const String& str);

	protected:
		String _name;
		MessageID _ID;
	};


	/**
		@brief Message.
	*/
	class SE_SAMPLEFRAMEWORK_EXPORT Message
	{
	public:
		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		Message(MessageDesc& desc);

		/** Destructor. */
		virtual ~Message();
		//@}

		MessageDesc GetDesc() const { return _Desc; }
		void SetDesc(MessageDesc value) { _Desc = value; }

		EntityID GetSource() const { return _Source; }
		void SetSource(EntityID value) { _Source = value; }

		EntityID GetDestination() const { return _Destination; }
		void SetDestination(EntityID value) { _Destination = value; }

	protected:
		MessageDesc _Desc;
		EntityID _Source;
		EntityID _Destination;
	};

	#define FW_DECLARE_MESSAGE(name) \
		static MessageDesc Message_##name;

	#define FW_IMPLEMENT_MESSAGE(classname, name) \
	MessageDesc classname::Message_##name(#name, MessageDesc::GenerateIDFromString(#name));

	#define FW_CHECK_MESSAGE(name) \
	if (msg.GetDesc().GetID() == Message_##name.GetID())
}

#endif

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
=============================================================================*/

/*=============================================================================
Entity.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_ENTITY_H_
#define _SFW_ENTITY_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Messaging/MessageHandler.h"

namespace SampleFramework
{
	class SE_SAMPLEFRAMEWORK_EXPORT Entity : public RefObject, public MessageHandler
	{
		SE_DECLARE_CLASS(Entity, Object);

		SE_BEGIN_REFLECTION(Entity);
			SE_Field(_name, String, Public);
		SE_END_REFLECTION(Entity);

	public:
		FW_DECLARE_MESSAGE(Update);
		FW_DECLARE_MESSAGE(Render);

		/** Destructor. */
		virtual ~Entity();

		virtual bool Equals(const Object* obj);
		virtual String ToString() const;
		virtual void Serialize(BinarySerializer* context, BinaryStream& stream);
		virtual void Deserialize(BinarySerializer* context, BinaryStream& stream);
		virtual void Serialize(XMLSerializer* context, XMLElement* element);
		virtual void Deserialize(XMLSerializer* context, XMLElement* element);

		/**
			Retrieves the name of the entity.
			@return Name of the entity.
		*/
		String GetName() const { return _name; }

		/**
			Sets the name of the entity.
			@param value Name of the entity.
		*/
		void SetName(const String& value) { _name = value; }

		/**
			Retrieves the user data associated with the entity.
			@return The user data associated with the entity.
		*/
		Variant& GetUserData() { return _userData; }

		/**
			Sets the user data associated with the entity.
			@param value The user data to associate with the entity.
		*/
		void SetUserData(Variant& value) { _userData = value; }

		EntityID GetID() const { return _ID; }
		void SetID(EntityID value) { _ID = value; }

		virtual void HandleMessage(Message& msg);

	protected:
		/** Protected constructor. */
		Entity();
		Entity(const String& name);

	protected:
		String _name;
		Variant _userData;
		EntityID _ID;
	};

	#include "Entity.inl"
}

#endif 

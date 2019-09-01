/*=============================================================================
Entity.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Entity.h"

namespace SampleFramework
{
	SE_IMPLEMENT_CLASS(Entity);
	SE_IMPLEMENT_REFLECTION(Entity);

	FW_IMPLEMENT_MESSAGE(Entity, Update);
	FW_IMPLEMENT_MESSAGE(Entity, Render);

	Entity::Entity() :
		RefObject()
	{
	}

	Entity::Entity(const String& name) :
		RefObject(),
		_name(name)
	{
	}

	Entity::~Entity()
	{
	}

	void Entity::HandleMessage(Message& msg)
	{
	}
}

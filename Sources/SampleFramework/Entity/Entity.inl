/*=============================================================================
Entity.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE bool Entity::Equals(const Object* obj)
{
	const Entity* value = SE_DynamicCast(obj, Entity);
	return (_name == value->GetName() && _ID == value->GetID());
}

SE_INLINE String Entity::ToString() const
{
	return _name;
}

SE_INLINE void Entity::Serialize(BinarySerializer* context, BinaryStream& stream)
{
	//stream >> _name >> _ID;
}

SE_INLINE void Entity::Deserialize(BinarySerializer* context, BinaryStream& stream)
{
	//stream << _name << _ID;
}

SE_INLINE void Entity::Serialize(XMLSerializer* context, XMLElement* element)
{
	element->SetAttribute("Name", _name);
	element->SetAttribute("ID", String::ToString(_ID));
}

SE_INLINE void Entity::Deserialize(XMLSerializer* context, XMLElement* element)
{
	_name = element->GetAttribute("Name");
	_ID = element->GetAttribute("ID").ToInt32();
}

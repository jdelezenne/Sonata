/*=============================================================================
Object.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Object::Object()
{
}

SE_INLINE Object::~Object()
{
}

SE_INLINE bool Object::Equals(const Object& left, const Object& right)
{
	return left.Equals(right);
}

SE_INLINE bool Object::operator==(const Object& obj) const
{
	return Equals(obj);
}

SE_INLINE bool Object::operator!=(const Object& obj) const
{
	return !(*this == obj);
}

SE_INLINE String Object::ToString() const
{
#if SE_USE_REFLECTION
	return Object::GetTypeStaticRef().GetName();
#else
	return _T("Object");
#endif
}


SE_INLINE RefObject::RefObject() :
	RefCounter()
{
}

SE_INLINE RefObject::~RefObject()
{
}


SE_INLINE NamedObject::NamedObject() :
	RefObject()
{
}

SE_INLINE NamedObject::~NamedObject()
{
}

/*=============================================================================
Ray3.inl
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

SE_INLINE Ray3::Ray3()
{
}

SE_INLINE Ray3::Ray3(const Vector3& origin, const Vector3& direction)
{
	Origin = origin;
	Direction = direction;
}

SE_INLINE Ray3::Ray3(const Ray3& value)
{
	Origin = value.Origin;
	Direction = value.Direction;
}

SE_INLINE bool Ray3::operator==(const Ray3& value) const
{
	return ((Origin == value.Origin) && (Direction == value.Direction));
}

SE_INLINE bool Ray3::operator!=(const Ray3& value) const
{
	return !(*this == value);
}

SE_INLINE Vector3 Ray3::ComputePoint(real t) const
{
	return (Origin + Direction * t);
}

SE_INLINE real Ray3::GetDistance(const Vector3& value, real* t) const
{
	return Math::Sqrt(GetDistanceSquared(value, t));
}

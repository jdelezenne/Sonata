/*=============================================================================
Ray2.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Ray2::Ray2()
{
}

SE_INLINE Ray2::Ray2(const Vector2& origin, const Vector2& direction)
{
	Origin = origin;
	Direction = direction;
}

SE_INLINE Ray2::Ray2(const Ray2& value)
{
	Origin = value.Origin;
	Direction = value.Direction;
}

SE_INLINE bool Ray2::operator==(const Ray2& value) const
{
	return ((Origin == value.Origin) && (Direction == value.Direction));
}

SE_INLINE bool Ray2::operator!=(const Ray2& value) const
{
	return !(*this == value);
}

SE_INLINE Vector2 Ray2::ComputePoint(real t) const
{
	return (Origin + Direction * t);
}

SE_INLINE real Ray2::GetDistance(const Vector2& value, real* t) const
{
	return Math::Sqrt(GetSquareDistance(value, t));
}

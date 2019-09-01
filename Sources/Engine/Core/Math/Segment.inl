/*=============================================================================
Segment.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Segment::Segment()
{
}

SE_INLINE Segment::Segment(const Vector3& p0, const Vector3& p1)
{
	this->p0 = p0;
	this->p1 = p1;
}

SE_INLINE Segment::Segment(const Segment& value)
{
	this->p0 = value.p0;
	this->p1 = value.p1;
}

SE_INLINE bool Segment::operator==(const Segment& value) const
{
	return ((p0 == value.p0) && (p1 == value.p1));
}

SE_INLINE bool Segment::operator!=(const Segment& value) const
{
	return !(*this == value);
}

SE_INLINE Vector3 Segment::GetOrigin() const
{
	return p0;
}

SE_INLINE Vector3 Segment::GetDirection() const
{
	return p1 - p0;
}

SE_INLINE void Segment::GetOriginDirection(Vector3& origin, Vector3& direction) const
{
	origin = p1;
	direction = p1 - p0;
}

SE_INLINE void Segment::SetOriginDirection(const Vector3& origin, const Vector3& direction)
{
	p0 = origin;
	p1 = origin + direction;
}

SE_INLINE real Segment::GetLength() const
{
	return p0.Distance(p1);
}

SE_INLINE real Segment::GetLengthSquared() const
{
	return p0.DistanceSquared(p1);
}

SE_INLINE Vector3 Segment::ComputePoint(real t) const
{
	return (p0 + t * (p1 - p0));
}

SE_INLINE real Segment::GetDistance(const Vector3& value, real* t) const
{
	return Math::Sqrt(GetSquareDistance(value, t));
}

SE_INLINE real Segment::GetSquareDistance(const Vector3& value, real* t) const
{
	//TODO: same as ray3
	return 0.0;
}

SE_INLINE void Segment::Transform(const Matrix4& transform)
{
	*this = Segment::Transform(*this, transform);
}

SE_INLINE Segment Segment::Transform(const Segment& segment, const Matrix4& transform)
{
	Segment result;

	// Transform the points
	result.p0 = Vector3::Transform(segment.p0, transform);
	result.p1 = Vector3::Transform(segment.p1, transform);

	return result;
}

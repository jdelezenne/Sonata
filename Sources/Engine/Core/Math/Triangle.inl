/*=============================================================================
Triangle.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Triangle::Triangle()
{
	this->p0 = Vector3::Zero;
	this->p1 = Vector3::Zero;
	this->p2 = Vector3::Zero;
}

SE_INLINE Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
}

SE_INLINE Triangle::Triangle(const Vector3 p[3])
{
	this->p0 = p[0];
	this->p1 = p[1];
	this->p2 = p[2];
}

SE_INLINE Triangle::Triangle(const Triangle& value)
{
	this->p0 = value.p0;
	this->p1 = value.p1;
	this->p2 = value.p2;
}

SE_INLINE bool Triangle::operator==(const Triangle& value) const
{
	return ((p0 == value.p0) && (p1 == value.p1) && (p2 == value.p2));
}

SE_INLINE bool Triangle::operator!=(const Triangle& value) const
{
	return !(*this == value);
}

SE_INLINE real Triangle::GetArea() const
{
	return (Vector3::Cross((p0 - p1), (p0 - p2)).Length() * (real)0.5);
}

SE_INLINE real Triangle::GetPerimeter() const
{
	return (p0.Distance(p1) + p0.Distance(p2) + p1.Distance(p2));
}

SE_INLINE Vector3 Triangle::GetBaryCentric(real u, real v) const
{
	return (p0 + u * (p1 - p0) + v * (p2 - p0));
}

SE_INLINE Vector3 Triangle::GetNormal() const
{
	Vector3 normal = Vector3::Cross((p0 - p1), (p0 - p2));
	normal.Normalize();
	return normal;
}

SE_INLINE Vector3 Triangle::GetCenter() const
{
	return ((p0 + p1 + p2) * (real)(1.0/3.0));
}

SE_INLINE Plane Triangle::GetPlane() const
{
	return Plane(p0, p1, p2);
}

SE_INLINE void Triangle::Flip()
{
	Vector3 temp = p2;
	p2 = p0;
	p0 = temp;
}

SE_INLINE real Triangle::GetDistance(const Vector3& value) const
{
	return 0.0;
}

SE_INLINE bool Triangle::ContainsPoint(const Vector3& value) const
{
	return false;
}

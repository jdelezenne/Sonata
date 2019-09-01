/*=============================================================================
Plane.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Plane::Plane()
{
	Normal = Vector3::UnitY;
	D = (real)0.0;
}

SE_INLINE Plane::Plane(real a, real b, real c, real d)
{
	Normal = Vector3(a, b, c);
	D = d;
}

SE_INLINE Plane::Plane(const Vector3& normal, real d)
{
	Normal = normal;
	D = d;
}

SE_INLINE Plane::Plane(const Vector3& normal, const Vector3& point)
{
	// a*x + b*y + c*z + d = 0
	// d = -(a*x + b*y + c*z) = -dot(p, n)
	Normal = normal;
	D = -Vector3::Dot(normal, point);
}

SE_INLINE Plane::Plane(const Vector4& value)
{
	Normal = value;
	D = value.W;
}

SE_INLINE Plane::Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	Vector3 edge0 = (p1 - p0);
	Vector3 edge1 = (p2 - p0);
	Normal = Vector3::Normalize(Vector3::Cross(edge0, edge1));
	D = -Vector3::Dot(Normal, p0);
}

SE_INLINE Plane::Plane(const Plane& value)
{
	Normal = value.Normal;
	D = value.D;
}

SE_INLINE bool Plane::operator==(const Plane& value) const
{
	return ((Normal == value.Normal) && (D == value.D));
}

SE_INLINE bool Plane::operator!=(const Plane& value) const
{
	return !(*this == value);
}

SE_INLINE real Plane::GetDistance(const Vector3& value) const
{
	// a*x + b*y + c*z + d = 0
	// dot(n, p) + d = 0
	return Vector3::Dot(Normal, value) + D;
}

SE_INLINE bool Plane::ContainsPoint(const Vector3& value) const
{
	return (Math::Abs(GetDistance(value)) < Math::Epsilon);
}

SE_INLINE PlaneIntersectionType Plane::ClassifyPoint(const Vector3& value) const
{
	real d = GetDistance(value);
	if (d < Math::Epsilon)
	{
		return PlaneIntersectionType_Back;
	}
	else if (d > Math::Epsilon)
	{
		return PlaneIntersectionType_Front;
	}
	else
	{
		return PlaneIntersectionType_Intersecting;
	}
}

SE_INLINE Vector3 Plane::Project(const Vector3& value) const
{
	return (value - Normal * GetDistance(value));
}

SE_INLINE void Plane::Flip()
{
	Normal = -Normal;
}

SE_INLINE void Plane::Normalize()
{
	real len = Normal.Length();
	if (len != (real)0.0)
	{
		Normal /= len;
		D /= len;
	}
}

SE_INLINE void Plane::Transform(const Matrix4& transform)
{
	*this = Plane::Transform(*this, transform);
}

SE_INLINE Plane Plane::Transform(const Plane& plane, const Matrix4& transform)
{
	Vector3 point;
	Plane result;

	// Put a point on the plane
	point = -plane.Normal * plane.D;

	// Transform the point
	point = Vector3::Transform(point, transform);

	// Rotate the normal
	result.Normal = Vector3::Transform(plane.Normal, transform);

	// Compute new distance
	result.D = plane.D - Vector3::Dot(result.Normal, point);

	return result;
}

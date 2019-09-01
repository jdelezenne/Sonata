/*=============================================================================
BoundingSphere.inl
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

SE_INLINE BoundingSphere::BoundingSphere()
{
	Center = Vector3::Zero;
	Radius = (real)0.0;
}

SE_INLINE BoundingSphere::BoundingSphere(const Vector3& center, real radius)
{
	Center = center;
	Radius = radius;
}

SE_INLINE BoundingSphere::BoundingSphere(const BoundingSphere& value)
{
	Center = value.Center;
	Radius = value.Radius;
}

SE_INLINE bool BoundingSphere::operator==(const BoundingSphere& value) const
{
	return ((Center == value.Center) && (Radius == value.Radius));
}

SE_INLINE bool BoundingSphere::operator!=(const BoundingSphere& value) const
{
	return !(*this == value);
}

SE_INLINE bool BoundingSphere::IsValid() const
{
	return (Radius >= (real)0.0);
}

SE_INLINE real BoundingSphere::GetArea() const
{
	return (real)4.0 * Math::Pi * Math::Pow(Radius, (real)2.0);
}

SE_INLINE real BoundingSphere::GetVolume() const
{
	return (real)(4.0/3.0) * Math::Pi * Math::Pow(Radius, (real)3.0);
}

SE_INLINE BoundingSphere BoundingSphere::Transform(const BoundingSphere& sphere, const Matrix4& transform)
{
	BoundingSphere result = sphere;
	result.Transform(transform);
	return result;
}

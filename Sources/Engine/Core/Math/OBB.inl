/*=============================================================================
OBB.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE OBB::OBB()
{
	Center = Vector3::Zero;
	Extents = Vector3::Zero;
	Rotation = Matrix3::Identity;
}

SE_INLINE OBB::OBB(const Vector3& center, const Vector3& extents, const Matrix3& rotation)
{
	Center = center;
	Extents = extents;
	Rotation = rotation;
}

SE_INLINE OBB::OBB(const AABB& aabb, const Matrix3& rotation)
{
	Center = aabb.GetCenter();
	Extents = aabb.GetExtents();
	Rotation = rotation;
}

SE_INLINE OBB::OBB(const OBB& value)
{
	Center = value.Center;
	Extents = value.Extents;
	Rotation = value.Rotation;
}

SE_INLINE bool OBB::IsValid() const
{
	// Invalid if any component of the extents < 0
	return ((Extents.X < (real)0.0) ||
		(Extents.Y < (real)0.0) ||
		(Extents.Z < (real)0.0));
}

SE_INLINE real OBB::GetArea() const
{
	return (real)2.0 * (Extents.X * Extents.Y +
		Extents.Y * Extents.Z + Extents.Z * Extents.X);
}

SE_INLINE real OBB::GetVolume() const
{
	return (Extents.X * Extents.Y * Extents.Z);
}

SE_INLINE const Vector3 OBB::GetCenter() const
{
	return Center;
}

SE_INLINE const Vector3 OBB::GetMinimum() const
{
	return Rotation.Transform((Center - Extents));
}

SE_INLINE const Vector3 OBB::GetMaximum() const
{
	return Rotation.Transform((Center + Extents));
}

SE_INLINE const Vector3 OBB::GetExtents() const
{
	return Extents;
}

SE_INLINE const Matrix3 OBB::GetRotation() const
{
	return Rotation;
}

SE_INLINE void OBB::ExpandTo(const Vector3& value)
{
	Vector3 rel = Rotation.Transform((value - Center));
	Extents = Vector3::Min(Extents, rel);
}

SE_INLINE bool OBB::Intersects(const Vector3& value) const
{
	Vector3 rel = Rotation.Transform((value - Center));

	return (rel.X >= -Extents.X && rel.X <= Extents.X &&
		rel.Y >= -Extents.Y && rel.Y <= Extents.Y &&
		rel.Z >= -Extents.Z && rel.Z <= Extents.Z);
}

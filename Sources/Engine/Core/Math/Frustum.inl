/*=============================================================================
Frustum.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Frustum::Frustum()
{
}

SE_INLINE Frustum::Frustum(const Matrix4& value)
{
	SetFrustumMatrix(value);
}

SE_INLINE Plane Frustum::GetFrustumPlane(FrustumPlane value) const
{
	if ((int)value >= 0 && (int)value < NumFrustumPlanes)
	{
		return _planes[value];
	}
	else
	{
		return Plane();
	}
}

SE_INLINE Plane Frustum::GetNear() const
{
	return _planes[FrustumPlane_Near];
}

SE_INLINE Plane Frustum::GetFar() const
{
	return _planes[FrustumPlane_Far];
}

SE_INLINE Plane Frustum::GetLeft() const
{
	return _planes[FrustumPlane_Left];
}

SE_INLINE Plane Frustum::GetRight() const
{
	return _planes[FrustumPlane_Right];
}

SE_INLINE Plane Frustum::GetTop() const
{
	return _planes[FrustumPlane_Top];
}

SE_INLINE Plane Frustum::GetBottom() const
{
	return _planes[FrustumPlane_Bottom];
}

SE_INLINE Matrix4 Frustum::GetMatrix() const
{
	return _matrix;
}

SE_INLINE void Frustum::SetMatrix(const Matrix4& value)
{
	SetFrustumMatrix(value);
}

SE_INLINE Vector3* Frustum::GetCorners() const
{
	return (Vector3*)_corners;
}

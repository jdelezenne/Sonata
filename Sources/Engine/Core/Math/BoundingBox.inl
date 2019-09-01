/*=============================================================================
BoundingBox.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE BoundingBox::BoundingBox()
{
	Min = Vector3::Zero;
	Max = Vector3::Zero;
}

SE_INLINE BoundingBox::BoundingBox(const Vector3& min, const Vector3& max)
{
	Min = min;
	Max = max;
}

SE_INLINE BoundingBox::BoundingBox(const BoundingBox& value)
{
	Min = value.Min;
	Max = value.Max;
}

SE_INLINE bool BoundingBox::IsEmpty() const
{
	if(Min.X < Max.X)
		return false;
	if(Min.Y < Max.Y)
		return false;
	if(Min.Z < Max.Z)
		return false;

	return true;
}

SE_INLINE const Vector3 BoundingBox::GetCenter() const
{
	return ((Max + Min) * (real)0.5);
}

SE_INLINE const Vector3 BoundingBox::GetDimensions() const
{
	return (Max - Min);
}

SE_INLINE const Vector3 BoundingBox::GetExtents() const
{
	return ((Max - Min) * (real)0.5);
}

SE_INLINE void BoundingBox::Merge(const BoundingBox& value)
{
	Min = Vector3::Min(Min, value.Min);
	Max = Vector3::Max(Max, value.Max);
}

SE_INLINE void BoundingBox::Scale(real value)
{
	Min *= value;
	Max *= value;
}

SE_INLINE void BoundingBox::ExpandTo(const Vector3& value)
{
	Min = Vector3::Min(Min, value);
	Max = Vector3::Max(Max, value);
}

SE_INLINE BoundingBox BoundingBox::Transform(const BoundingBox& box, const Matrix4& transform)
{
	BoundingBox result = box;
	result.Transform(transform);
	return result;
}

/*=============================================================================
AxisAngle.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE AxisAngle::AxisAngle()
{
	Axis = Vector3::Zero;
	Angle = (real)0.0;
}

SE_INLINE AxisAngle::AxisAngle(real x, real y, real z, real angle)
{
	Axis = Vector3(x, y, z);
	Angle = angle;
}

SE_INLINE AxisAngle::AxisAngle(const Vector3& axis, real angle)
{
	Axis = axis;
	Angle = angle;
}

SE_INLINE AxisAngle::AxisAngle(const AxisAngle& value)
{
	Axis = value.Axis;
	Angle = value.Angle;
}

SE_INLINE bool AxisAngle::operator==(const AxisAngle& value) const
{
	return ((Axis == value.Axis) && (Angle == value.Angle));
}

SE_INLINE bool AxisAngle::operator!=(const AxisAngle& value) const
{
	return !(*this == value);
}

SE_INLINE void AxisAngle::ToEulerAngles(EulerAngles& value)
{
	//TODO
	value.FromAxisAngle(Axis, Angle);
}

SE_INLINE void AxisAngle::ToMatrix3(Matrix3& value)
{
	//TODO
	//value.FromAxisAngle(Axis, Angle);
}

SE_INLINE void AxisAngle::ToQuaternion(Quaternion& value)
{
	value = Quaternion::CreateFromAxisAngle(Axis, Angle);
}

SE_INLINE bool AxisAngle::Equals(const AxisAngle& value) const
{
    return (*this == value);
}

SE_INLINE bool AxisAngle::Equals(const AxisAngle& value, real epsilon) const
{
	if (Math::Abs(Angle - value.Angle) > epsilon)
	{
		return false;
	}
	if (!Axis.Equals(value.Axis, epsilon))
	{
		return false;
	}
	return true;
}

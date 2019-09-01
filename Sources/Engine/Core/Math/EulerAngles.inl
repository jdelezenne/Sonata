/*=============================================================================
EulerAngles.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE EulerAngles::EulerAngles()
{
	X = Y = Z = (real)0.0;
	Order = EulerAnglesOrder_XYZ;
}

SE_INLINE EulerAngles::EulerAngles(const Vector3& angles, EulerAnglesOrder order)
{
	X = angles.X;
	Y = angles.Y;
	Z = angles.Z;
	Order = order;
}

SE_INLINE EulerAngles::EulerAngles(real x, real y, real z, EulerAnglesOrder order)
{
	X = x;
	Y = y;
	Z = z;
	Order = order;
}

SE_INLINE EulerAngles::EulerAngles(real angles, EulerAnglesOrder order)
{
	X = Y = Z = angles;
	Order = order;
}

SE_INLINE EulerAngles::EulerAngles(const EulerAngles& value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	Order = value.Order;
}

SE_INLINE bool EulerAngles::operator==(const EulerAngles& value) const
{
	return ((X == value.X) && (Y == value.Y) && (Z == value.Z) && (Order == value.Order));
}

SE_INLINE bool EulerAngles::operator!=(const EulerAngles& value) const
{
	return !(*this == value);
}

SE_INLINE real EulerAngles::operator[](int index) const
{
	assert(0 <= index && index < 3);
	return Data[index];
}

SE_INLINE real& EulerAngles::operator[](int index)
{
	assert(0 <= index && index < 3);
	return Data[index];
}

SE_INLINE EulerAngles EulerAngles::operator+() const
{
	return *this;
}

SE_INLINE EulerAngles EulerAngles::operator-() const
{
	return EulerAngles(-X, -Y, -Z, Order);
}

SE_INLINE EulerAngles& EulerAngles::operator+=(const EulerAngles& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
	return *this;
}

SE_INLINE EulerAngles& EulerAngles::operator-=(const EulerAngles& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
	return *this;
}

SE_INLINE EulerAngles& EulerAngles::operator*=(real value)
{
	X *= value;
	Y *= value;
	Z *= value;
	return *this;
}

SE_INLINE EulerAngles& EulerAngles::operator/=(real value)
{
	real inv = (real)1.0 / value;
	X *= inv;
	Y *= inv;
	Z *= inv;
	return *this;
}

SE_INLINE EulerAngles EulerAngles::operator+(const EulerAngles& value) const
{
	return EulerAngles(X + value.X, Y + value.Y, Z + value.Z, Order);
}

SE_INLINE EulerAngles EulerAngles::operator-(const EulerAngles& value) const
{
	return EulerAngles(X - value.X, Y - value.Y, Z + value.Z, Order);
}

SE_INLINE EulerAngles EulerAngles::operator*(real value) const
{
	return EulerAngles(X * value, Y * value, Z * value, Order);
}

SE_INLINE EulerAngles EulerAngles::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return EulerAngles(X * inv, Y * inv, Z * inv, Order);
}

SE_INLINE EulerAngles EulerAngles::Clamp(const EulerAngles& value, const EulerAngles& min, const EulerAngles& max)
{
	return EulerAngles(
		Math::Clamp(value.X, min.X, max.X),
		Math::Clamp(value.Y, min.Y, max.Y),
		Math::Clamp(value.Z, min.Z, max.Z),
		value.Order);
}

SE_INLINE void EulerAngles::ToVector(Vector3& value) const
{
	value = Vector3(X, Y, Z);
}

SE_INLINE bool EulerAngles::Equals(const EulerAngles& value) const
{
    return (*this == value);
}

SE_INLINE bool EulerAngles::Equals(const EulerAngles& value, real epsilon) const
{
	if (Order != value.Order)
	{
		return false;
	}
	if (Math::Abs(X - value.X) > epsilon)
	{
		return false;
	}
	if (Math::Abs(Y - value.Y) > epsilon)
	{
		return false;
	}
	if (Math::Abs(Z - value.Z) > epsilon)
	{
		return false;
	}
	return true;
}

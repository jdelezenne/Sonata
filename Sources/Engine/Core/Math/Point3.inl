/*=============================================================================
Point3.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Point3::Point3()
{
}

SE_INLINE Point3::Point3(real x, real y, real z)
{
	X = x;
	Y = y;
	Z = z;
}

SE_INLINE Point3::Point3(real value)
{
	X = Y = Z = value;
}

SE_INLINE Point3::Point3(const real* value)
{
	X = value[0];
	Y = value[1];
	Z = value[2];
}

SE_INLINE Point3::Point3(const Vector3& value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
}

SE_INLINE Point3::Point3(const Point3& value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
}

SE_INLINE Point3::operator const real*() const
{
	return (const real*)Data;
}

SE_INLINE Point3::operator real*()
{
	return (real*)Data;
}

SE_INLINE real Point3::operator[](int index) const
{
	SE_ASSERT(0 <= index && index < 3);
	return Data[index];
}

SE_INLINE real& Point3::operator[](int index)
{
	SE_ASSERT(0 <= index && index < 3);
	return Data[index];
}

SE_INLINE Point3 Point3::operator+() const
{
	return *this;
}

SE_INLINE Point3 Point3::operator-() const
{
	return Point3(-X, -Y, -Z);
}

SE_INLINE Point3& Point3::operator+=(const Point3& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
	return *this;
}

SE_INLINE Point3& Point3::operator-=(const Point3& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
	return *this;
}

SE_INLINE Point3& Point3::operator*=(const Point3& value)
{
	X *= value.X;
	Y *= value.Y;
	Z *= value.Z;
	return *this;
}

SE_INLINE Point3& Point3::operator*=(real value)
{
	X *= value;
	Y *= value;
	Z *= value;
	return *this;
}

SE_INLINE Point3& Point3::operator/=(const Point3& value)
{
	X /= value.X;
	Y /= value.Y;
	Z /= value.Z;
	return *this;
}

SE_INLINE Point3& Point3::operator/=(real value)
{
	real inv = (real)1.0 / value;
	X *= inv;
	Y *= inv;
	Z *= inv;
	return *this;
}

SE_INLINE Point3 Point3::operator+(const Point3& value) const
{
	return Point3(X + value.X, Y + value.Y, Z + value.Z);
}

SE_INLINE Point3 Point3::operator-(const Point3& value) const
{
	return Point3(X - value.X, Y - value.Y, Z - value.Z);
}

SE_INLINE Point3 Point3::operator*(const Point3& value) const
{
	return Point3(X * value.X, Y * value.Y, Z * value.Z);
}

SE_INLINE Point3 Point3::operator*(real value) const
{
	return Point3(X * value, Y * value, Z * value);
}

SE_INLINE Point3 Point3::operator/(const Point3& value) const
{
	return Point3(X / value.X, Y / value.Y, Z / value.Z);
}

SE_INLINE Point3 Point3::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return Point3(X * inv, Y * inv, Z * inv);
}

SE_INLINE bool Point3::operator==(const Point3& value) const
{
	return ((X == value.X) && (Y == value.Y) && (Z == value.Z));
}

SE_INLINE bool Point3::operator!=(const Point3& value) const
{
	return !(*this == value);
}

SE_INLINE Point3 operator+(real left, const Point3& right)
{
	return Point3(left + right.X, left + right.Y, left + right.Z);
}

SE_INLINE Point3 operator-(real left, const Point3& right)
{
	return Point3(left - right.X, left - right.Y, left - right.Z);
}

SE_INLINE Point3 operator*(real left, const Point3& right)
{
	return Point3(left * right.X, left * right.Y, left * right.Z);
}

SE_INLINE Point3 operator/(real left, const Point3& right)
{
	return Point3(left / right.X, left / right.Y, left / right.Z);
}

SE_INLINE Point3 Point3::Negate(const Point3& value)
{
	return -value;
}

SE_INLINE Point3 Point3::Add(const Point3& value1, const Point3& value2)
{
	return (value1 + value2);
}

SE_INLINE Point3 Point3::Substract(const Point3& value1, const Point3& value2)
{
	return (value1 - value2);
}

SE_INLINE Point3 Point3::Multiply(const Point3& value1, const Point3& value2)
{
	return (value1 * value2);
}

SE_INLINE Point3 Point3::Multiply(const Point3& value1, real value2)
{
	return (value1 * value2);
}

SE_INLINE Point3 Point3::Divide(const Point3& value1, const Point3& value2)
{
	return (value1 / value2);
}

SE_INLINE Point3 Point3::Divide(const Point3& value1, real value2)
{
	return (value1 / value2);
}

SE_INLINE real Point3::Sum() const
{
	return (X * X + Y * Y + Z * Z);
}

SE_INLINE void Point3::Min(const Point3& value)
{
	X = Math::Min(X, value.X);
	Y = Math::Min(Y, value.Y);
	Z = Math::Min(Z, value.Z);
}

SE_INLINE Point3 Point3::Min(const Point3& left, const Point3& right)
{
	return Point3(
		Math::Min(left.X, right.X),
		Math::Min(left.Y, right.Y),
		Math::Min(left.Z, right.Z));
}

SE_INLINE void Point3::Max(const Point3& value)
{
	X = Math::Max(X, value.X);
	Y = Math::Max(Y, value.Y);
	Z = Math::Max(Z, value.Z);
}

SE_INLINE Point3 Point3::Max(const Point3& left, const Point3& right)
{
	return Point3(
		Math::Max(left.X, right.X),
		Math::Max(left.Y, right.Y),
		Math::Max(left.Z, right.Z));
}

SE_INLINE Point3 Point3::Clamp(const Point3& value, const Point3& min, const Point3& max)
{
	return Point3(
		Math::Clamp(value.X, min.X, max.X),
		Math::Clamp(value.Y, min.Y, max.Y),
		Math::Clamp(value.Z, min.Z, max.Z));
}

SE_INLINE Point3 Point3::Lerp(const Point3& value1, const Point3& value2, real amount)
{
	return Point3(
		Math::Lerp(value1.X, value2.X, amount),
		Math::Lerp(value1.Y, value2.Y, amount),
		Math::Lerp(value1.Z, value2.Z, amount));
}

SE_INLINE Point3 Point3::SmoothStep(const Point3& value1, const Point3& value2, real amount)
{
	Point3 result;
	amount = Math::Clamp(amount, (real)0.0, (real)1.0);
	amount = (amount * amount) * (3.0 - (2.0 * amount));
	result.X = value1.X + ((value2.X - value1.X) * amount);
	result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
	result.Z = value1.Z + ((value2.Z - value1.Z) * amount);
	return result;
}

SE_INLINE Point3 Point3::CatmullRom(const Point3& value1, const Point3& value2, const Point3& value3, const Point3& value4, real amount)
{
	Point3 result;
	real amount2 = amount * amount;
	real amount3 = amount * amount2;

	result.X = 0.5 * (
		(2.0 * value2.X) +
		(-value1.X + value3.X) * amount +
		((2.0 * value1.X) - (5.0 * value2.X) + (4.0 * value3.X) - value4.X) * amount2 +
		(-value1.X + (3.0 * value2.X) - (3.0 * value3.X) + value4.X) * amount3);
	result.Y = 0.5 * (
		(2.0 * value2.Y) +
		(-value1.Y + value3.Y) * amount +
		((2.0 * value1.Y) - (5.0 * value2.Y) + (4.0 * value3.Y) - value4.Y) * amount2 +
		(-value1.Y + (3.0 * value2.Y) - (3.0 * value3.Y) + value4.Y) * amount3);
	result.Z = 0.5 * (
		(2.0 * value2.Z) +
		(-value1.Z + value3.Z) * amount +
		((2.0 * value1.Z) - (5.0 * value2.Z) + (4.0 * value3.Z) - value4.Z) * amount2 +
		(-value1.Z + (3.0 * value2.Z) - (3.0 * value3.Z) + value4.Z) * amount3);

	return result;
}

SE_INLINE Point3 Point3::Hermite(const Point3& value1, const Point3& tangent1, const Point3& value2, const Point3& tangent2, real amount)
{
	Point3 result;
	real amount2 = amount * amount;
	real amount3 = amount * amount2;
	real a = ((2.0 * amount3) - (3.0 * amount2)) + 1.0;
	real b = (-2.0 * amount3) + (3.0 * amount2);
	real c = (amount3 - (2.0 * amount2)) + amount;
	real d = amount3 - amount2;

	result.X = (value1.X * a) + (value2.X * b) + (tangent1.X * c) + (tangent2.X * d);
	result.Y = (value1.Y * a) + (value2.Y * b) + (tangent1.Y * c) + (tangent2.Y * d);
	result.Z = (value1.Z * a) + (value2.Z * b) + (tangent1.Z * c) + (tangent2.Z * d);
	return result;
}

SE_INLINE Point3 Point3::Barycentric(const Point3& value1, const Point3& value2, const Point3& value3, real amount1, real amount2)
{
	Point3 result;
	result.X = value1.X + (amount1 * (value2.X - value1.X)) + (amount2 * (value3.X - value1.X));
	result.Y = value1.Y + (amount1 * (value2.Y - value1.Y)) + (amount2 * (value3.Y - value1.Y));
	result.Z = value1.Z + (amount1 * (value2.Z - value1.Z)) + (amount2 * (value3.Z - value1.Z));
	return result;
}

SE_INLINE bool Point3::Equals(const Point3& value) const
{
    return (*this == value);
}

SE_INLINE bool Point3::Equals(const Point3& value, real epsilon) const
{
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

SE_INLINE int32 Point3::GetHashCode() const
{
	return (RealType(X).GetHashCode() ^ RealType(Y).GetHashCode() ^ RealType(Z).GetHashCode());
}

SE_INLINE String Point3::ToString() const
{
	return String::Format(_T("(%f,%f,%f)"), X, Y, Z);
}

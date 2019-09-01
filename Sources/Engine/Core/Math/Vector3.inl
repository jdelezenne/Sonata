/*=============================================================================
Vector3.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Vector3::Vector3()
{
}

SE_INLINE Vector3::Vector3(real x, real y, real z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

SE_INLINE Vector3::Vector3(real value)
{
	X = Y = Z = value;
}

SE_INLINE Vector3::Vector3(const real* value)
{
	X = value[0];
	Y = value[1];
	Z = value[2];
}

SE_INLINE Vector3::Vector3(const Vector2& value, real z)
{
	X = value.X;
	Y = value.Y;
	this->Z = z;
}

SE_INLINE Vector3::Vector3(const Vector3& value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
}

SE_INLINE Vector3::operator const real*() const
{
	return (const real*)Data;
}

SE_INLINE Vector3::operator real*()
{
	return (real*)Data;
}

SE_INLINE real Vector3::operator[](int index) const
{
	SE_ASSERT(0 <= index && index < 3);
	return Data[index];
}

SE_INLINE real& Vector3::operator[](int index)
{
	SE_ASSERT(0 <= index && index < 3);
	return Data[index];
}

SE_INLINE Vector3 Vector3::operator+() const
{
	return *this;
}

SE_INLINE Vector3 Vector3::operator-() const
{
	return Vector3(-X, -Y, -Z);
}

SE_INLINE Vector3& Vector3::operator+=(const Vector3& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
	return *this;
}

SE_INLINE Vector3& Vector3::operator-=(const Vector3& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
	return *this;
}

SE_INLINE Vector3& Vector3::operator*=(const Vector3& value)
{
	X *= value.X;
	Y *= value.Y;
	Z *= value.Z;
	return *this;
}

SE_INLINE Vector3& Vector3::operator*=(real value)
{
	X *= value;
	Y *= value;
	Z *= value;
	return *this;
}

SE_INLINE Vector3& Vector3::operator/=(const Vector3& value)
{
	X /= value.X;
	Y /= value.Y;
	Z /= value.Z;
	return *this;
}

SE_INLINE Vector3& Vector3::operator/=(real value)
{
	real inv = (real)1.0 / value;
	X *= inv;
	Y *= inv;
	Z *= inv;
	return *this;
}

SE_INLINE Vector3 Vector3::operator+(const Vector3& value) const
{
	return Vector3(X + value.X, Y + value.Y, Z + value.Z);
}

SE_INLINE Vector3 Vector3::operator-(const Vector3& value) const
{
	return Vector3(X - value.X, Y - value.Y, Z - value.Z);
}

SE_INLINE Vector3 Vector3::operator*(const Vector3& value) const
{
	return Vector3(X * value.X, Y * value.Y, Z * value.Z);
}

SE_INLINE Vector3 Vector3::operator*(real value) const
{
	return Vector3(X * value, Y * value, Z * value);
}

SE_INLINE Vector3 Vector3::operator/(const Vector3& value) const
{
	return Vector3(X / value.X, Y / value.Y, Z / value.Z);
}

SE_INLINE Vector3 Vector3::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return Vector3(X * inv, Y * inv, Z * inv);
}

SE_INLINE bool Vector3::operator==(const Vector3& value) const
{
	return ((X == value.X) && (Y == value.Y) && (Z == value.Z));
}

SE_INLINE bool Vector3::operator!=(const Vector3& value) const
{
	return !(*this == value);
}

SE_INLINE Vector3 operator+(real left, const Vector3& right)
{
	return Vector3(left + right.X, left + right.Y, left + right.Z);
}

SE_INLINE Vector3 operator-(real left, const Vector3& right)
{
	return Vector3(left - right.X, left - right.Y, left - right.Z);
}

SE_INLINE Vector3 operator*(real left, const Vector3& right)
{
	return Vector3(left * right.X, left * right.Y, left * right.Z);
}

SE_INLINE Vector3 operator/(real left, const Vector3& right)
{
	return Vector3(left / right.X, left / right.Y, left / right.Z);
}

SE_INLINE Vector3 Vector3::Negate(const Vector3& value)
{
	return -value;
}

SE_INLINE Vector3 Vector3::Add(const Vector3& value1, const Vector3& value2)
{
	return (value1 + value2);
}

SE_INLINE Vector3 Vector3::Substract(const Vector3& value1, const Vector3& value2)
{
	return (value1 - value2);
}

SE_INLINE Vector3 Vector3::Multiply(const Vector3& value1, const Vector3& value2)
{
	return (value1 * value2);
}

SE_INLINE Vector3 Vector3::Multiply(const Vector3& value1, real value2)
{
	return (value1 * value2);
}

SE_INLINE Vector3 Vector3::Divide(const Vector3& value1, const Vector3& value2)
{
	return (value1 / value2);
}

SE_INLINE Vector3 Vector3::Divide(const Vector3& value1, real value2)
{
	return (value1 / value2);
}

SE_INLINE real Vector3::Length() const
{
	return Math::Sqrt(X * X + Y * Y + Z * Z);
}

SE_INLINE real Vector3::LengthSquared() const
{
	return (X * X + Y * Y + Z * Z);
}

SE_INLINE real Vector3::Distance(const Vector3& value) const
{
	Vector3 diff = (*this - value);
	return diff.Length();
}

SE_INLINE real Vector3::Distance(const Vector3& left, const Vector3& right)
{
	return left.Distance(right);
}

SE_INLINE real Vector3::DistanceSquared(const Vector3& value) const
{
	Vector3 diff = (*this - value);
	return diff.LengthSquared();
}

SE_INLINE real Vector3::DistanceSquared(const Vector3& left, const Vector3& right)
{
	return left.DistanceSquared(right);
}

SE_INLINE void Vector3::Normalize()
{
	real len, inv;

	len = Length();
	if (fabs(len) < Math::Epsilon)
	{
		X = (real)0.0;
		Y = (real)0.0;
		Z = (real)0.0;
	}
	else
	{
		inv = (real)1.0 / len;
		X *= inv;
		Y *= inv;
		Z *= inv;
	}
}

SE_INLINE Vector3 Vector3::Normalize(const Vector3& value)
{
	Vector3 res = value;
	res.Normalize();
	return res;
}

SE_INLINE void Vector3::Scale(const Vector3& value)
{
	X *= value.X;
	Y *= value.Y;
	Z *= value.Z;
}

SE_INLINE Vector3 Vector3::Scaling(const Vector3& left, const Vector3& right)
{
	return Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
}

SE_INLINE real Vector3::Dot(const Vector3& value) const
{
	return (X * value.X + Y * value.Y + Z * value.Z);
}

SE_INLINE real Vector3::Dot(const Vector3& left, const Vector3& right)
{
	return (left.X * right.X + left.Y * right.Y + left.Z * right.Z);
}

SE_INLINE Vector3 Vector3::Cross(const Vector3& value) const
{
	return Vector3(
		Y * value.Z - Z * value.Y,
		Z * value.X - X * value.Z,
		X * value.Y - Y * value.X);
}

SE_INLINE Vector3 Vector3::Cross(const Vector3& left, const Vector3& right)
{
	return Vector3(
		left.Y * right.Z - left.Z * right.Y,
		left.Z * right.X - left.X * right.Z,
		left.X * right.Y - left.Y * right.X);
}

SE_INLINE real Vector3::Angle(const Vector3& value) const
{
	return Vector3::Angle(*this, value);
}

SE_INLINE real Vector3::Angle(const Vector3& left, const Vector3& right)
{
	real d = left.Length() * right.Length();
	if (d == (real)0.0)
		return (real)0.0;

	real c = Vector3::Dot(left, right) / d;
	if (c < (real)-1.0) c = (real)-1.0;
	if (c > (real)1.0) c = (real)1.0;

	return Math::Acos(c);
}

SE_INLINE bool Vector3::SameDirection(const Vector3& value) const
{
	return (Dot(value) >= (real)0.0);
}

SE_INLINE bool Vector3::SameDirection(const Vector3& left, const Vector3& right)
{
	return Vector3::Dot(left, right) >= (real)0.0;
}

SE_INLINE void Vector3::Min(const Vector3& value)
{
	X = Math::Min(X, value.X);
	Y = Math::Min(Y, value.Y);
	Z = Math::Min(Z, value.Z);
}

SE_INLINE Vector3 Vector3::Min(const Vector3& left, const Vector3& right)
{
	return Vector3(
		Math::Min(left.X, right.X),
		Math::Min(left.Y, right.Y),
		Math::Min(left.Z, right.Z));
}

SE_INLINE void Vector3::Max(const Vector3& value)
{
	X = Math::Max(X, value.X);
	Y = Math::Max(Y, value.Y);
	Z = Math::Max(Z, value.Z);
}

SE_INLINE Vector3 Vector3::Max(const Vector3& left, const Vector3& right)
{
	return Vector3(
		Math::Max(left.X, right.X),
		Math::Max(left.Y, right.Y),
		Math::Max(left.Z, right.Z));
}

SE_INLINE Vector3 Vector3::Clamp(const Vector3& value, const Vector3& min, const Vector3& max)
{
	return Vector3(
		Math::Clamp(value.X, min.X, max.X),
		Math::Clamp(value.Y, min.Y, max.Y),
		Math::Clamp(value.Z, min.Z, max.Z));
}

SE_INLINE Vector3 Vector3::Lerp(const Vector3& value1, const Vector3& value2, real amount)
{
	return Vector3(
		Math::Lerp(value1.X, value2.X, amount),
		Math::Lerp(value1.Y, value2.Y, amount),
		Math::Lerp(value1.Z, value2.Z, amount));
}

SE_INLINE Vector3 Vector3::SmoothStep(const Vector3& value1, const Vector3& value2, real amount)
{
	Vector3 result;
	amount = Math::Clamp(amount, (real)0.0, (real)1.0);
	amount = (amount * amount) * (3.0 - (2.0 * amount));
	result.X = value1.X + ((value2.X - value1.X) * amount);
	result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
	result.Z = value1.Z + ((value2.Z - value1.Z) * amount);
	return result;
}

SE_INLINE Vector3 Vector3::CatmullRom(const Vector3& value1, const Vector3& value2, const Vector3& value3, const Vector3& value4, real amount)
{
	Vector3 result;
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

SE_INLINE Vector3 Vector3::Hermite(const Vector3& value1, const Vector3& tangent1, const Vector3& value2, const Vector3& tangent2, real amount)
{
	Vector3 result;
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

SE_INLINE Vector3 Vector3::Barycentric(const Vector3& value1, const Vector3& value2, const Vector3& value3, real amount1, real amount2)
{
	Vector3 result;
	result.X = value1.X + (amount1 * (value2.X - value1.X)) + (amount2 * (value3.X - value1.X));
	result.Y = value1.Y + (amount1 * (value2.Y - value1.Y)) + (amount2 * (value3.Y - value1.Y));
	result.Z = value1.Z + (amount1 * (value2.Z - value1.Z)) + (amount2 * (value3.Z - value1.Z));
	return result;
}

SE_INLINE Vector3 Vector3::Reflect(const Vector3& value, const Vector3& normal)
{
	return value - (real)2.0 * normal * Vector3::Dot(value, normal);
}

SE_INLINE bool Vector3::Equals(const Vector3& value) const
{
    return (*this == value);
}

SE_INLINE bool Vector3::Equals(const Vector3& value, real epsilon) const
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

SE_INLINE int32 Vector3::GetHashCode() const
{
	return (RealType(X).GetHashCode() ^ RealType(Y).GetHashCode() ^ RealType(Z).GetHashCode());
}

SE_INLINE String Vector3::ToString() const
{
	return String::Format(_T("(%f,%f,%f)"), X, Y, Z);
}

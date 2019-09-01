/*=============================================================================
Vector2.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Vector2::Vector2()
{
}

SE_INLINE Vector2::Vector2(real x, real y)
{
	X = x;
	Y = y;
}

SE_INLINE Vector2::Vector2(real value)
{
	X = Y = value;
}

SE_INLINE Vector2::Vector2(const real* value)
{
	X = value[0];
	Y = value[1];
}

SE_INLINE Vector2::Vector2(const Vector2& value)
{
	X = value.X;
	Y = value.Y;
}

SE_INLINE Vector2::operator const real*() const
{
	return (const real*)Data;
}

SE_INLINE Vector2::operator real*()
{
	return (real*)Data;
}

SE_INLINE real Vector2::operator[](int index) const
{
	SE_ASSERT(0 <= index && index < 2);
	return Data[index];
}

SE_INLINE real& Vector2::operator[](int index)
{
	SE_ASSERT(0 <= index && index < 2);
	return Data[index];
}

SE_INLINE Vector2 Vector2::operator+() const
{
	return *this;
}

SE_INLINE Vector2 Vector2::operator-() const
{
	return Vector2(-X, -Y);
}

SE_INLINE Vector2& Vector2::operator+=(const Vector2& value)
{
	X += value.X;
	Y += value.Y;
	return *this;
}

SE_INLINE Vector2& Vector2::operator-=(const Vector2& value)
{
	X -= value.X;
	Y -= value.Y;
	return *this;
}

SE_INLINE Vector2& Vector2::operator*=(const Vector2& value)
{
	X *= value.X;
	Y *= value.Y;
	return *this;
}

SE_INLINE Vector2& Vector2::operator*=(real value)
{
	X *= value;
	Y *= value;
	return *this;
}

SE_INLINE Vector2& Vector2::operator/=(const Vector2& value)
{
	X /= value.X;
	Y /= value.Y;
	return *this;
}

SE_INLINE Vector2& Vector2::operator/=(real value)
{
	real inv = (real)1.0 / value;
	X *= inv;
	Y *= inv;
	return *this;
}

SE_INLINE Vector2 Vector2::operator+(const Vector2& value) const
{
	return Vector2(X + value.X, Y + value.Y);
}

SE_INLINE Vector2 Vector2::operator-(const Vector2& value) const
{
	return Vector2(X - value.X, Y - value.Y);
}

SE_INLINE Vector2 Vector2::operator*(const Vector2& value) const
{
	return Vector2(X * value.X, Y * value.Y);
}

SE_INLINE Vector2 Vector2::operator*(real value) const
{
	return Vector2(X * value, Y * value);
}

SE_INLINE Vector2 Vector2::operator/(const Vector2& value) const
{
	return Vector2(X / value.X, Y / value.Y);
}

SE_INLINE Vector2 Vector2::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return Vector2(X * inv, Y * inv);
}

SE_INLINE bool Vector2::operator==(const Vector2& value) const
{
	return ((X == value.X) && (Y == value.Y));
}

SE_INLINE bool Vector2::operator!=(const Vector2& value) const
{
	return !(*this == value);
}

SE_INLINE Vector2 operator+(real left, const Vector2& right)
{
	return Vector2(left + right.X, left + right.Y);
}

SE_INLINE Vector2 operator-(real left, const Vector2& right)
{
	return Vector2(left - right.X, left - right.Y);
}

SE_INLINE Vector2 operator*(real left, const Vector2& right)
{
	return Vector2(left * right.X, left * right.Y);
}

SE_INLINE Vector2 operator/(real left, const Vector2& right)
{
	return Vector2(left / right.X, left / right.Y);
}

SE_INLINE Vector2 Vector2::Negate(const Vector2& value)
{
	return -value;
}

SE_INLINE Vector2 Vector2::Add(const Vector2& value1, const Vector2& value2)
{
	return (value1 + value2);
}

SE_INLINE Vector2 Vector2::Substract(const Vector2& value1, const Vector2& value2)
{
	return (value1 - value2);
}

SE_INLINE Vector2 Vector2::Multiply(const Vector2& value1, const Vector2& value2)
{
	return (value1 * value2);
}

SE_INLINE Vector2 Vector2::Multiply(const Vector2& value1, real value2)
{
	return (value1 * value2);
}

SE_INLINE Vector2 Vector2::Divide(const Vector2& value1, const Vector2& value2)
{
	return (value1 / value2);
}

SE_INLINE Vector2 Vector2::Divide(const Vector2& value1, real value2)
{
	return (value1 / value2);
}

SE_INLINE real Vector2::Length() const
{
	return Math::Sqrt(X * X + Y * Y);
}

SE_INLINE real Vector2::LengthSquared() const
{
	return (X * X + Y * Y);
}

SE_INLINE real Vector2::Distance(const Vector2& value) const
{
	Vector2 diff = (*this - value);
	return diff.Length();
}

SE_INLINE real Vector2::Distance(const Vector2& left, const Vector2& right)
{
	return left.Distance(right);
}

SE_INLINE real Vector2::DistanceSquared(const Vector2& value) const
{
	Vector2 diff = (*this - value);
	return diff.LengthSquared();
}

SE_INLINE real Vector2::DistanceSquared(const Vector2& left, const Vector2& right)
{
	return left.DistanceSquared(right);
}

SE_INLINE void Vector2::Normalize()
{
	real len, inv;

	len = Length();
	if (Math::Abs(len) < Math::Epsilon)
	{
		X = (real)0.0;
		Y = (real)0.0;
	}
	else
	{
		inv = (real)1.0 / len;
		X *= inv;
		Y *= inv;
	}
}

SE_INLINE Vector2 Vector2::Normalize(const Vector2& value)
{
	Vector2 result = value;
	result.Normalize();
	return result;
}

SE_INLINE void Vector2::Scale(const Vector2& value)
{
	X *= value.X;
	Y *= value.Y;
}

SE_INLINE Vector2 Vector2::Scaling(const Vector2& left, const Vector2& right)
{
	return Vector2(left.X * right.X, left.Y * right.Y);
}

SE_INLINE real Vector2::Dot(const Vector2& value) const
{
	return Vector2::Dot(*this, value);
}

SE_INLINE real Vector2::Dot(const Vector2& left, const Vector2& right)
{
	return (left.X * right.X + left.Y * right.Y);
}

SE_INLINE real Vector2::Angle(const Vector2& value) const
{
	return Vector2::Angle(*this, value);
}

SE_INLINE real Vector2::Angle(const Vector2& left, const Vector2& right)
{
	real d = left.Length() * right.Length();
	if (d == (real)0.0)
		return (real)0.0;

	real c = Vector2::Dot(left, right) / d;
	if (c < (real)-1.0) c = (real)-1.0;
	if (c > (real)1.0) c = (real)1.0;

	return Math::Acos(c);
}

SE_INLINE bool Vector2::SameDirection(const Vector2& value) const
{
	return Dot(value) >= (real)0.0;
}

SE_INLINE bool Vector2::SameDirection(const Vector2& left, const Vector2& right)
{
	return Vector2::Dot(left, right) >= (real)0.0;
}

SE_INLINE void Vector2::Min(const Vector2& value)
{
	X = Math::Min(X, value.X);
	Y = Math::Min(Y, value.Y);
}

SE_INLINE Vector2 Vector2::Min(const Vector2& left, const Vector2& right)
{
	return Vector2(
		Math::Min(left.X, right.X),
		Math::Min(left.Y, right.Y));
}

SE_INLINE void Vector2::Max(const Vector2& value)
{
	X = Math::Max(X, value.X);
	Y = Math::Max(Y, value.Y);
}

SE_INLINE Vector2 Vector2::Max(const Vector2& left, const Vector2& right)
{
	return Vector2(
		Math::Max(left.X, right.X),
		Math::Max(left.Y, right.Y));
}

SE_INLINE Vector2 Vector2::Clamp(const Vector2& value, const Vector2& min, const Vector2& max)
{
	return Vector2(
		Math::Clamp(value.X, min.X, max.X),
		Math::Clamp(value.Y, min.Y, max.Y));
}

SE_INLINE Vector2 Vector2::Lerp(const Vector2& value1, const Vector2& value2, real amount)
{
	return Vector2(
		Math::Lerp(value1.X, value2.X, amount),
		Math::Lerp(value1.Y, value2.Y, amount));
}

SE_INLINE Vector2 Vector2::SmoothStep(const Vector2& value1, const Vector2& value2, real amount)
{
	Vector2 result;
	amount = Math::Clamp(amount, (real)0.0, (real)1.0);
	amount = (amount * amount) * (3.0 - (2.0 * amount));
	result.X = value1.X + ((value2.X - value1.X) * amount);
	result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
	return result;
}

SE_INLINE Vector2 Vector2::CatmullRom(const Vector2& value1, const Vector2& value2, const Vector2& value3, const Vector2& value4, real amount)
{
	Vector2 result;
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

	return result;
}

SE_INLINE Vector2 Vector2::Hermite(const Vector2& value1, const Vector2& tangent1, const Vector2& value2, const Vector2& tangent2, real amount)
{
	Vector2 result;
	real amount2 = amount * amount;
	real amount3 = amount * amount2;
	real a = ((2.0 * amount3) - (3.0 * amount2)) + 1.0;
	real b = (-2.0 * amount3) + (3.0 * amount2);
	real c = (amount3 - (2.0 * amount2)) + amount;
	real d = amount3 - amount2;

	result.X = (value1.X * a) + (value2.X * b) + (tangent1.X * c) + (tangent2.X * d);
	result.Y = (value1.Y * a) + (value2.Y * b) + (tangent1.Y * c) + (tangent2.Y * d);
	return result;
}

SE_INLINE Vector2 Vector2::Barycentric(const Vector2& value1, const Vector2& value2, const Vector2& value3, real amount1, real amount2)
{
	Vector2 result;
	result.X = value1.X + (amount1 * (value2.X - value1.X)) + (amount2 * (value3.X - value1.X));
	result.Y = value1.Y + (amount1 * (value2.Y - value1.Y)) + (amount2 * (value3.Y - value1.Y));
	return result;
}

SE_INLINE Vector2 Vector2::Reflect(const Vector2& value, const Vector2& normal)
{
	return value - (real)2.0 * normal * Vector2::Dot(value, normal);
}

SE_INLINE bool Vector2::Equals(const Vector2& value) const
{
    return (*this == value);
}

SE_INLINE bool Vector2::Equals(const Vector2& value, real epsilon) const
{
	if (Math::Abs(X - value.X) > epsilon)
	{
		return false;
	}
	if (Math::Abs(Y - value.Y) > epsilon)
	{
		return false;
	}
	return true;
}

SE_INLINE int32 Vector2::GetHashCode() const
{
	return (RealType(X).GetHashCode() ^ RealType(Y).GetHashCode());
}

SE_INLINE String Vector2::ToString() const
{
	return String::Format(_T("(%f,%f)"), X, Y);
}

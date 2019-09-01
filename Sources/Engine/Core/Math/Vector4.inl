/*=============================================================================
Vector4.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Vector4::Vector4()
{
}

SE_INLINE Vector4::Vector4(real x, real y, real z, real w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

SE_INLINE Vector4::Vector4(real value)
{
	X = Y = Z = W = value;
}

SE_INLINE Vector4::Vector4(const real* value)
{
	X = value[0];
	Y = value[1];
	Z = value[2];
	W = value[3];
}

SE_INLINE Vector4::Vector4(const Vector2& value, real z, real w)
{
	X = value.X;
	Y = value.Y;
	Z = z;
	W = w;
}

SE_INLINE Vector4::Vector4(const Vector3& value, real w)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	W = w;
}

SE_INLINE Vector4::Vector4(const Vector4& value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	W = value.W;
}

SE_INLINE Vector4::operator const real*() const
{
	return (const real*)Data;
}

SE_INLINE Vector4::operator real*()
{
	return (real*)Data;
}

SE_INLINE Vector4::operator const Vector3() const
{
	return Vector3(X, Y, Z);
}

SE_INLINE Vector4::operator Vector3()
{
	return Vector3(X, Y, Z);
}

SE_INLINE real Vector4::operator[](int index) const
{
	SE_ASSERT(0 <= index && index < 4);
	return Data[index];
}

SE_INLINE real& Vector4::operator[](int index)
{
	SE_ASSERT(0 <= index && index < 4);
	return Data[index];
}

SE_INLINE Vector4& Vector4::operator+=(const Vector4& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
	W += value.W;
	return *this;
}

SE_INLINE Vector4& Vector4::operator-=(const Vector4& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
	W -= value.W;
	return *this;
}

SE_INLINE Vector4& Vector4::operator*=(const Vector4& value)
{
	X *= value.X;
	Y *= value.Y;
	Z *= value.Z;
	W *= value.W;
	return *this;
}

SE_INLINE Vector4& Vector4::operator*=(real value)
{
	X *= value;
	Y *= value;
	Z *= value;
	W *= value;
	return *this;
}

SE_INLINE Vector4& Vector4::operator/=(const Vector4& value)
{
	X *= value.X;
	Y *= value.Y;
	Z *= value.Z;
	W *= value.W;
	return *this;
}

SE_INLINE Vector4& Vector4::operator/=(real value)
{
	real inv = (real)1.0 / value;
	X *= inv;
	Y *= inv;
	Z *= inv;
	W *= inv;
	return *this;
}

SE_INLINE Vector4 Vector4::operator+() const
{
	return *this;
}

SE_INLINE Vector4 Vector4::operator-() const
{
	return Vector4(-X, -Y, -Z, -W);
}

SE_INLINE Vector4 Vector4::operator+(const Vector4& value) const
{
	return Vector4(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
}

SE_INLINE Vector4 Vector4::operator-(const Vector4& value) const
{
	return Vector4(X - value.X, Y - value.Y, Z - value.Z, W - value.W);
}

SE_INLINE Vector4 Vector4::operator*(const Vector4& value) const
{
	return Vector4(X * value.X, Y * value.Y, Z * value.Z, W * value.W);
}

SE_INLINE Vector4 Vector4::operator*(real value) const
{
	return Vector4(X * value, Y * value, Z * value, W * value);
}

SE_INLINE Vector4 Vector4::operator/(const Vector4& value) const
{
	return Vector4(X / value.X, Y / value.Y, Z / value.Z, W / value.W);
}

SE_INLINE Vector4 Vector4::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return Vector4(X * inv, Y * inv, Z * inv, W * inv);
}

SE_INLINE bool Vector4::operator==(const Vector4& value) const
{
	return ((X == value.X) && (Y == value.Y) && (Z == value.Z) && (W == value.W));
}

SE_INLINE bool Vector4::operator!=(const Vector4& value) const
{
	return !(*this == value);
}

SE_INLINE Vector4 operator+(real left, const Vector4& right)
{
	return Vector4(left + right.X, left + right.Y, left + right.Z, left + right.W);
}

SE_INLINE Vector4 operator-(real left, const Vector4& right)
{
	return Vector4(left - right.X, left - right.Y, left - right.Z, left - right.W);
}

SE_INLINE Vector4 operator*(real left, const Vector4& right)
{
	return Vector4(left * right.X, left * right.Y, left * right.Z, left * right.W);
}

SE_INLINE Vector4 operator/(real left, const Vector4& right)
{
	return Vector4(left / right.X, left / right.Y, left / right.Z, left / right.W);
}

SE_INLINE Vector4 Vector4::Negate(const Vector4& value)
{
	return -value;
}

SE_INLINE Vector4 Vector4::Add(const Vector4& value1, const Vector4& value2)
{
	return (value1 + value2);
}

SE_INLINE Vector4 Vector4::Substract(const Vector4& value1, const Vector4& value2)
{
	return (value1 - value2);
}

SE_INLINE Vector4 Vector4::Multiply(const Vector4& value1, const Vector4& value2)
{
	return (value1 * value2);
}

SE_INLINE Vector4 Vector4::Multiply(const Vector4& value1, real value2)
{
	return (value1 * value2);
}

SE_INLINE Vector4 Vector4::Divide(const Vector4& value1, const Vector4& value2)
{
	return (value1 / value2);
}

SE_INLINE Vector4 Vector4::Divide(const Vector4& value1, real value2)
{
	return (value1 / value2);
}

SE_INLINE real Vector4::Length() const
{
	return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
}

SE_INLINE real Vector4::LengthSquared() const
{
	return (X * X + Y * Y + Z * Z + W * W);
}

SE_INLINE real Vector4::Distance(const Vector4& value) const
{
	Vector4 diff = (*this - value);
	return diff.Length();
}

SE_INLINE real Vector4::Distance(const Vector4& left, const Vector4& right)
{
	return left.Distance(right);
}

SE_INLINE real Vector4::DistanceSquared(const Vector4& value) const
{
	Vector4 diff = (*this - value);
	return diff.LengthSquared();
}

SE_INLINE real Vector4::DistanceSquared(const Vector4& left, const Vector4& right)
{
	return left.DistanceSquared(right);
}

SE_INLINE void Vector4::Normalize()
{
	real len, inv;

	len = Length();
	if (fabs(len) < Math::Epsilon)
	{
		X = (real)0.0;
		Y = (real)0.0;
		Z = (real)0.0;
		W = (real)0.0;
	}
	else
	{
		inv = (real)1.0 / len;
		X *= inv;
		Y *= inv;
		Z *= inv;
		W *= inv;
	}
}

SE_INLINE Vector4 Vector4::Normalize(const Vector4& value)
{
	Vector4 result = value;
	result.Normalize();
	return result;
}

SE_INLINE void Vector4::Scale(const Vector4& value)
{
	X *= value.X;
	Y *= value.Y;
	Z *= value.Z;
	W *= value.W;
}

SE_INLINE Vector4 Vector4::Scaling(const Vector4& left, const Vector4& right)
{
	return Vector4(left.X * right.X, left.Y * right.Y, left.Z * right.Z, left.W * right.W);
}

SE_INLINE real Vector4::Dot(const Vector4& value) const
{
	return (X * value.X + Y * value.Y + Z * value.Z);
}

SE_INLINE real Vector4::Dot(const Vector4& left, const Vector4& right)
{
	return (left.X * right.X + left.Y * right.Y + left.Z * right.Z);
}

SE_INLINE real Vector4::Angle(const Vector4& value) const
{
	return Vector4::Angle(*this, value);
}

SE_INLINE real Vector4::Angle(const Vector4& left, const Vector4& right)
{
	real d = left.Length() * right.Length();
	if (d == (real)0.0)
		return (real)0.0;

	real c = Vector4::Dot(left, right) / d;
	if (c < (real)-1.0) c = (real)-1.0;
	if (c > (real)1.0) c = (real)1.0;

	return Math::Acos(c);
}

SE_INLINE bool Vector4::SameDirection(const Vector4& value) const
{
	return Dot(value) >= (real)0.0;
}

SE_INLINE bool Vector4::SameDirection(const Vector4& left, const Vector4& right)
{
	return Vector4::Dot(left, right) >= (real)0.0;
}

SE_INLINE void Vector4::Min(const Vector4& value)
{
	X = Math::Min(X, value.X);
	Y = Math::Min(Y, value.Y);
	Z = Math::Min(Z, value.Z);
	W = Math::Min(W, value.W);
}

SE_INLINE Vector4 Vector4::Min(const Vector4& left, const Vector4& right)
{
	return Vector4(
		Math::Min(left.X, right.X),
		Math::Min(left.Y, right.Y),
		Math::Min(left.Z, right.Z),
		Math::Min(left.W, right.W));
}

SE_INLINE void Vector4::Max(const Vector4& value)
{
	X = Math::Max(X, value.X);
	Y = Math::Max(Y, value.Y);
	Z = Math::Max(Z, value.Z);
	W = Math::Max(W, value.W);
}

SE_INLINE Vector4 Vector4::Max(const Vector4& left, const Vector4& right)
{
	return Vector4(
		Math::Max(left.X, right.X),
		Math::Max(left.Y, right.Y),
		Math::Max(left.Z, right.Z),
		Math::Max(left.W, right.W));
}

SE_INLINE Vector4 Vector4::Clamp(const Vector4& value, const Vector4& min, const Vector4& max)
{
	return Vector4(
		Math::Clamp(value.X, min.X, max.X),
		Math::Clamp(value.Y, min.Y, max.Y),
		Math::Clamp(value.Z, min.Z, max.Z),
		Math::Clamp(value.W, min.W, max.W));
}

SE_INLINE Vector4 Vector4::Lerp(const Vector4& value1, const Vector4& value2, real amount)
{
	return Vector4(
		Math::Lerp(value1.X, value2.X, amount),
		Math::Lerp(value1.Y, value2.Y, amount),
		Math::Lerp(value1.Z, value2.Z, amount),
		Math::Lerp(value1.W, value2.W, amount));
}

SE_INLINE Vector4 Vector4::SmoothStep(const Vector4& value1, const Vector4& value2, real amount)
{
	Vector4 result;
	amount = Math::Clamp(amount, (real)0.0, (real)1.0);
	amount = (amount * amount) * (3.0 - (2.0 * amount));
	result.X = value1.X + ((value2.X - value1.X) * amount);
	result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
	result.Z = value1.Z + ((value2.Z - value1.Z) * amount);
	result.W = value1.W + ((value2.W - value1.W) * amount);
	return result;
}

SE_INLINE Vector4 Vector4::CatmullRom(const Vector4& value1, const Vector4& value2, const Vector4& value3, const Vector4& value4, real amount)
{
	Vector4 result;
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
	result.W = 0.5 * (
		(2.0 * value2.W) +
		(-value1.W + value3.W) * amount +
		((2.0 * value1.W) - (5.0 * value2.W) + (4.0 * value3.W) - value4.W) * amount2 +
		(-value1.W + (3.0 * value2.W) - (3.0 * value3.W) + value4.W) * amount3);

	return result;
}

SE_INLINE Vector4 Vector4::Hermite(const Vector4& value1, const Vector4& tangent1, const Vector4& value2, const Vector4& tangent2, real amount)
{
	Vector4 result;
	real amount2 = amount * amount;
	real amount3 = amount * amount2;
	real a = ((2.0 * amount3) - (3.0 * amount2)) + 1.0;
	real b = (-2.0 * amount3) + (3.0 * amount2);
	real c = (amount3 - (2.0 * amount2)) + amount;
	real d = amount3 - amount2;

	result.X = (value1.X * a) + (value2.X * b) + (tangent1.X * c) + (tangent2.X * d);
	result.Y = (value1.Y * a) + (value2.Y * b) + (tangent1.Y * c) + (tangent2.Y * d);
	result.Z = (value1.Z * a) + (value2.Z * b) + (tangent1.Z * c) + (tangent2.Z * d);
	result.W = (value1.W * a) + (value2.W * b) + (tangent1.W * c) + (tangent2.W * d);
	return result;
}

SE_INLINE Vector4 Vector4::Barycentric(const Vector4& value1, const Vector4& value2, const Vector4& value3, real amount1, real amount2)
{
	Vector4 result;
	result.X = value1.X + (amount1 * (value2.X - value1.X)) + (amount2 * (value3.X - value1.X));
	result.Y = value1.Y + (amount1 * (value2.Y - value1.Y)) + (amount2 * (value3.Y - value1.Y));
	result.Z = value1.Z + (amount1 * (value2.Z - value1.Z)) + (amount2 * (value3.Z - value1.Z));
	result.W = value1.W + (amount1 * (value2.W - value1.W)) + (amount2 * (value3.W - value1.W));
	return result;
}

SE_INLINE Vector4 Vector4::Reflect(const Vector4& value, const Vector4& normal)
{
	return value - (real)2.0 * normal * Vector4::Dot(value, normal);
}

SE_INLINE bool Vector4::Equals(const Vector4& value) const
{
    return (*this == value);
}

SE_INLINE bool Vector4::Equals(const Vector4& value, real epsilon) const
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
	if (Math::Abs(W - value.W) > epsilon)
	{
		return false;
	}
	return true;
}

SE_INLINE int32 Vector4::GetHashCode() const
{
	return (RealType(X).GetHashCode() ^ RealType(Y).GetHashCode() ^
		RealType(Z).GetHashCode() ^ RealType(W).GetHashCode());
}

SE_INLINE String Vector4::ToString() const
{
	return String::Format(_T("(%f,%f,%f,%f)"), X, Y, Z, W);
}

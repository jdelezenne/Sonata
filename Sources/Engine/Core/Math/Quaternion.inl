/*=============================================================================
Quaternion.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Quaternion::Quaternion()
{
}

SE_INLINE Quaternion::Quaternion(real x, real y, real z, real w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

SE_INLINE Quaternion::Quaternion(const Vector3& value, real w)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	W = w;
}

SE_INLINE Quaternion::Quaternion(const Quaternion& value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	W = value.W;
}

SE_INLINE Quaternion::operator const real*() const
{
	return (const real*)Data;
}

SE_INLINE Quaternion::operator real*()
{
	return (real*)Data;
}

SE_INLINE real Quaternion::operator[](int index) const
{
	SE_ASSERT(0 <= index && index < 4);
	return Data[index];
}

SE_INLINE real& Quaternion::operator[](int index)
{
	SE_ASSERT(0 <= index && index < 4);
	return Data[index];
}

SE_INLINE Quaternion& Quaternion::operator+=(const Quaternion& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
	W += value.W;
	return *this;
}

SE_INLINE Quaternion& Quaternion::operator-=(const Quaternion& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
	W -= value.W;
	return *this;
}

SE_INLINE Quaternion& Quaternion::operator*=(const Quaternion& value)
{
	*this = (*this * value);
	return *this;
}

SE_INLINE Quaternion& Quaternion::operator*=(real value)
{
	X *= value;
	Y *= value;
	Z *= value;
	W *= value;
	return *this;
}

SE_INLINE Quaternion& Quaternion::operator/=(const Quaternion& value)
{
	*this = (*this / Quaternion::Invert(value));
	return *this;
}

SE_INLINE Quaternion& Quaternion::operator/=(real value)
{
	real inv = (real)1.0 / value;
	X *= inv;
	Y *= inv;
	Z *= inv;
	W *= inv;
	return *this;
}

SE_INLINE Quaternion Quaternion::operator+() const
{
	return *this;
}

SE_INLINE Quaternion Quaternion::operator-() const
{
	return Quaternion(-X, -Y, -Z, -W);
}

SE_INLINE Quaternion Quaternion::operator+(const Quaternion& value) const
{
	return Quaternion(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
}

SE_INLINE Quaternion Quaternion::operator-(const Quaternion& value) const
{
	return Quaternion(X - value.X, Y - value.Y, Z - value.Z, W - value.W);
}

SE_INLINE Quaternion Quaternion::operator*(const Quaternion& value) const
{
	return Quaternion(
		W*value.X + X*value.W + Y*value.Z - Z*value.Y,
		W*value.Y + Y*value.W + Z*value.X - X*value.Z,
		W*value.Z + Z*value.W + X*value.Y - Y*value.X,
		W*value.W - X*value.X - Y*value.Y - Z*value.Z);
}

SE_INLINE Quaternion Quaternion::operator*(real value) const
{
	return Quaternion(X * value, Y * value, Z * value, W * value);
}

SE_INLINE Quaternion Quaternion::operator/(const Quaternion& value) const
{
	return Quaternion(
		W*value.X + X*value.W + Y*value.Z - Z*value.Y,
		W*value.Y + Y*value.W + Z*value.X - X*value.Z,
		W*value.Z + Z*value.W + X*value.Y - Y*value.X,
		W*value.W - X*value.X - Y*value.Y - Z*value.Z);
}

SE_INLINE Quaternion Quaternion::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return Quaternion(X * inv, Y * inv, Z * inv, W * inv);
}

SE_INLINE bool Quaternion::operator==(const Quaternion& value) const
{
	return ((X == value.X) && (Y == value.Y) && (Z == value.Z) && (W == value.W));
}

SE_INLINE bool Quaternion::operator!=(const Quaternion& value) const
{
	return !(*this == value);
}

SE_INLINE void Quaternion::SetIdentity()
{
	X = Y = Z = (real)0.0;
	W = (real)1.0;
}

SE_INLINE bool Quaternion::IsIdentity() const
{
	return ((X == (real)0.0) && (Y == (real)0.0) && (Z == (real)0.0) && (W == (real)1.0));
}

SE_INLINE Quaternion Quaternion::Negate(const Quaternion& value)
{
	return -value;
}

SE_INLINE void Quaternion::Add(const Quaternion& value)
{
	*this = (*this + value);
}

SE_INLINE Quaternion Quaternion::Add(const Quaternion& left, const Quaternion& right)
{
	return (left + right);
}

SE_INLINE void Quaternion::Substract(const Quaternion& value)
{
	*this = (*this - value);
}

SE_INLINE Quaternion Quaternion::Substract(const Quaternion& left, const Quaternion& right)
{
	return (left - right);
}

SE_INLINE void Quaternion::Multiply(const Quaternion& value)
{
	*this = (*this * value);
}

SE_INLINE Quaternion Quaternion::Multiply(const Quaternion& left, const Quaternion& right)
{
	return (left * right);
}

SE_INLINE void Quaternion::Divide(const Quaternion& value)
{
	*this = (*this / value);
}

SE_INLINE Quaternion Quaternion::Divide(const Quaternion& left, const Quaternion& right)
{
	return (left / right);
}

SE_INLINE real Quaternion::Length()
{
	return Math::Sqrt(X*X + Y*Y + Z*Z + W*W);
}

SE_INLINE real Quaternion::LengthSquared()
{
	return (X*X + Y*Y + Z*Z + W*W);
}

SE_INLINE Quaternion Quaternion::Normalize(const Quaternion& value)
{
	Quaternion result(value);
	result.Normalize();
	return result;
}

SE_INLINE real Quaternion::Dot(const Quaternion& value) const
{
	return (X * value.X + Y * value.Y + Z * value.Z + W * value.W);
}

SE_INLINE real Quaternion::Dot(const Quaternion& left, const Quaternion& right)
{
	return (left.X * right.X + left.Y * right.Y + left.Z * right.Z + left.W * right.W);
}

SE_INLINE void Quaternion::Conjugate()
{
	X = -X;
	Y = -Y;
	Z = -Z;
}

SE_INLINE Quaternion Quaternion::Conjugate(const Quaternion& value)
{
	Quaternion result;
	result.X = -value.X;
	result.Y = -value.Y;
	result.Z = -value.Z;
	result.W =	value.W;
	return result;
}

SE_INLINE Quaternion Quaternion::Invert(const Quaternion& value)
{
	Quaternion result(value);
	result.Invert();
	return result;
}

SE_INLINE Quaternion Quaternion::Log(const Quaternion& value)
{
	Quaternion result(value);
	result.Log();
	return result;
}

SE_INLINE Quaternion Quaternion::Exp(const Quaternion& value)
{
	Quaternion result(value);
	result.Exp();
	return result;
}

SE_INLINE Vector3 Quaternion::Rotate(const Vector3& vector) const
{
	return Quaternion::Rotate(*this, vector);
}

SE_INLINE Vector3 Quaternion::Rotate(const Quaternion& quaternion, const Vector3& vector)
{
	Vector3 uv, uuv;
	Vector3 v(quaternion.X, quaternion.Y, quaternion.Z);
	uv = v.Cross(vector); 
	uuv = v.Cross(uv); 
	uv *= ((real)2.0 * quaternion.W); 
	uuv *= (real)2.0; 

	return (vector + uv + uuv);
}

/*=============================================================================
Matrix3.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Matrix3::Matrix3()
{
}

SE_INLINE Matrix3::Matrix3(const Matrix3& value)
{
	Memory::Copy(Data, (void*)&value, sizeof(Matrix3));
}

SE_INLINE Matrix3::Matrix3(
	real m00, real m01, real m02,
	real m10, real m11, real m12,
	real m20, real m21, real m22)
{
	M00 = m00; M01 = m01; M02 = m02;
	M10 = m10; M11 = m11; M12 = m12;
	M20 = m20; M21 = m21; M22 = m22;
}

SE_INLINE Matrix3::Matrix3(const real value[9])
{
	Memory::Copy(Data, (void*)value, 9 * sizeof(real));
}

SE_INLINE real Matrix3::operator()(int row, int col) const
{
	return M[row][col];
}

SE_INLINE real& Matrix3::operator()(int row, int col)
{
	return M[row][col];
}

SE_INLINE Matrix3::operator const real*() const
{
	return (const real*)Data;
}

SE_INLINE Matrix3::operator real*()
{
	return (real*)Data;
}

SE_INLINE Matrix3& Matrix3::operator+=(const Matrix3& value)
{
	M00 += value.M00; M01 += value.M01; M02 += value.M02;
	M10 += value.M10; M11 += value.M11; M12 += value.M12;
	M20 += value.M20; M21 += value.M21; M22 += value.M22;
	return *this;
}

SE_INLINE Matrix3& Matrix3::operator-=(const Matrix3& value)
{
	M00 -= value.M00; M01 -= value.M01; M02 -= value.M02;
	M10 -= value.M10; M11 -= value.M11; M12 -= value.M12;
	M20 -= value.M20; M21 -= value.M21; M22 -= value.M22;
	return *this;
}

SE_INLINE Matrix3& Matrix3::operator*=(const Matrix3& value)
{
	*this = (*this * value);
	return *this;
}

SE_INLINE Matrix3& Matrix3::operator*=(real value)
{
	M00 *= value; M01 *= value; M02 *= value;
	M10 *= value; M11 *= value; M12 *= value;
	M20 *= value; M21 *= value; M22 *= value;
	return *this;
}

SE_INLINE Matrix3& Matrix3::operator/=(real value)
{
	real inv = (real)1.0 / value;
	M00 *= inv; M01 *= inv; M02 *= inv;
	M10 *= inv; M11 *= inv; M12 *= inv;
	M20 *= inv; M21 *= inv; M22 *= inv;
	return *this;
}

SE_INLINE Matrix3 Matrix3::operator+() const
{
	return *this;
}

SE_INLINE Matrix3 Matrix3::operator-() const
{
	return Matrix3(
		-M00, -M01, -M02,
		-M10, -M11, -M12,
		-M20, -M21, -M22);
}

SE_INLINE Matrix3 Matrix3::operator+(const Matrix3& value) const
{
	return Matrix3(
		M00 + value.M00, M01 + value.M01, M02 + value.M02,
		M10 + value.M10, M11 + value.M11, M12 + value.M12,
		M20 + value.M20, M21 + value.M21, M22 + value.M22);
}

SE_INLINE Matrix3 Matrix3::operator-(const Matrix3& value) const
{
	return Matrix3(
		M00 - value.M00, M01 - value.M01, M02 - value.M02,
		M10 - value.M10, M11 - value.M11, M12 - value.M12,
		M20 - value.M20, M21 - value.M21, M22 - value.M22);
}

SE_INLINE Matrix3 Matrix3::operator*(const Matrix3& value) const
{
	Matrix3 result;
	int i, j, k;

	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 3; i++)
		{
			result.M[j][i] = (real)0.0;
			for (k = 0; k < 3; k++)
				result.M[j][i] += M[j][k] * value.M[k][i];
		}
	}

	return result;
}

SE_INLINE Matrix3 Matrix3::operator*(real value) const
{
	return Matrix3(
		M00 * value, M01 * value, M02 * value,
		M10 * value, M11 * value, M12 * value,
		M20 * value, M21 * value, M22 * value);
}

SE_INLINE Matrix3 Matrix3::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return Matrix3(
		M00 * inv, M01 * inv, M02 * inv,
		M10 * inv, M11 * inv, M12 * inv,
		M20 * inv, M21 * inv, M22 * inv);
}

SE_INLINE bool Matrix3::operator==(const Matrix3& value) const
{
	return (Memory::Compare((void*)this, (void*)&value, sizeof(Matrix3)) == 0);
}

SE_INLINE bool Matrix3::operator!=(const Matrix3& value) const
{
	return !(*this == value);
}

SE_INLINE Vector3 Matrix3::GetRow(int row) const
{
	return *(Vector3*)&Data[3*row];
}

SE_INLINE void Matrix3::SetRow(int row, const Vector3& value)
{
	M[row][0] = value.X;
	M[row][1] = value.Y;
	M[row][2] = value.Z;
}

SE_INLINE Vector3 Matrix3::GetColumn(int column) const
{
	return Vector3(M[0][column], M[1][column], M[2][column]);
}

SE_INLINE void Matrix3::SetColumn(int column, const Vector3& value)
{
	M[0][column] = value.X;
	M[1][column] = value.Y;
	M[2][column] = value.Z;
}

SE_INLINE void Matrix3::SetIdentity()
{
	M[0][1] = M[0][2] =
	M[1][0] = M[1][2] =
	M[2][0] = M[2][1] = (real)0.0;
	M[0][0] = M[1][1] = M[2][2] = (real)1.0;
}

SE_INLINE bool Matrix3::IsIdentity() const
{
	return (
		M[0][0] == (real)1.0 && M[0][1] == (real)0.0 && M[0][2] == (real)0.0 &&
		M[1][0] == (real)0.0 && M[1][1] == (real)1.0 && M[1][2] == (real)0.0 &&
		M[2][0] == (real)0.0 && M[2][1] == (real)0.0 && M[2][2] == (real)1.0);
}

SE_INLINE void Matrix3::Transpose()
{
	*this = Matrix3::Transpose(*this);
}

SE_INLINE Matrix3 Matrix3::Transpose(const Matrix3& value)
{
	Matrix3 result;
	int i, j;

	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 3; i++)
		{
			result.M[j][i] = value.M[i][j];
		}
	}

	return result;
}

SE_INLINE void Matrix3::Add(const Matrix3& value)
{
	*this = (*this + value);
}

SE_INLINE Matrix3 Matrix3::Add(const Matrix3& left, const Matrix3& right)
{
	return (left + right);
}

SE_INLINE void Matrix3::Substract(const Matrix3& value)
{
	*this = (*this - value);
}

SE_INLINE Matrix3 Matrix3::Substract(const Matrix3& left, const Matrix3& right)
{
	return (left - right);
}

SE_INLINE void Matrix3::Multiply(const Matrix3& value)
{
	*this = (*this * value);
}

SE_INLINE Matrix3 Matrix3::Multiply(const Matrix3& left, const Matrix3& right)
{
	return (left * right);
}
	
SE_INLINE void Matrix3::MultiplyTranspose(const Matrix3& value)
{
	Multiply(value);
	Transpose();
}

SE_INLINE Matrix3 Matrix3::MultiplyTranspose(const Matrix3& left, const Matrix3& right)
{
	Matrix3 result;
	result = Matrix3::Multiply(left, right);
	result.Transpose();
	return result;
}

SE_INLINE void Matrix3::Invert()
{
	*this = Matrix3::Invert(*this);
}

SE_INLINE Vector3 Matrix3::Transform(const Vector3& value) const
{
	Vector3 result;

	result.X = M[0][0] * value.X + M[0][1] * value.Y + M[0][2] * value.Z;
	result.Y = M[1][0] * value.X + M[1][1] * value.Y + M[1][2] * value.Z;
	result.Z = M[2][0] * value.X + M[2][1] * value.Y + M[2][2] * value.Z;

	return result;
}

SE_INLINE void Matrix3::RotateX(real angle)
{
	(*this) *= Matrix3::CreateRotationX(angle);
}

SE_INLINE void Matrix3::RotateY(real angle)
{
	(*this) *= Matrix3::CreateRotationY(angle);
}

SE_INLINE void Matrix3::RotateZ(real angle)
{
	(*this) *= Matrix3::CreateRotationZ(angle);
}

SE_INLINE void Matrix3::Scale(real x, real y, real z)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		M[0][i] *= x;
		M[1][i] *= y;
		M[2][i] *= z;
	}
}

SE_INLINE void Matrix3::Scale(const Vector3& value)
{
	Scale(value.X, value.Y, value.Z);
}

SE_INLINE Matrix3 Matrix3::CreateRotationX(real angle)
{
	Matrix3 result;
	real fSin, fCos;

	fSin = Math::Sin(angle);
	fCos = Math::Cos(angle);

	result.M00 = (real)1.0;
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	
	result.M10 = (real)0.0;
	result.M11 = fCos;
	result.M12 = -fSin;
	
	result.M20 = (real)0.0;
	result.M21 = fSin;
	result.M22 = fCos;

	return result;
}

SE_INLINE Matrix3 Matrix3::CreateRotationY(real angle)
{
	Matrix3 result;
	real fSin, fCos;

	fSin = Math::Sin(angle);
	fCos = Math::Cos(angle);
	
	result.M00 = fCos;
	result.M01 = (real)0.0;
	result.M02 = fSin;
	
	result.M10 = (real)0.0;
	result.M11 = (real)1.0;
	result.M12 = (real)0.0;
	
	result.M20 = -fSin;
	result.M21 = (real)0.0;
	result.M22 = fCos;

	return result;
}

SE_INLINE Matrix3 Matrix3::CreateRotationZ(real angle)
{
	Matrix3 result;
	real fSin, fCos;

	fSin = Math::Sin(angle);
	fCos = Math::Cos(angle);
	
	result.M00 = fCos;
	result.M01 = -fSin;
	result.M02 = (real)0.0;
	
	result.M10 = fSin;
	result.M11 = fCos;
	result.M12 = (real)0.0;
	
	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = (real)1.0;

	return result;
}

SE_INLINE Matrix3 Matrix3::CreateScale(real x, real y, real z)
{
	Matrix3 result;
	result.M[0][0] = x;
	result.M[0][1] = 0.0;
	result.M[0][2] = 0.0;

	result.M[1][0] = 0.0;
	result.M[1][1] = y;
	result.M[1][2] = 0.0;

	result.M[2][0] = 0.0;
	result.M[2][1] = 0.0;
	result.M[2][2] = z;

	return result;
}

SE_INLINE Matrix3 Matrix3::CreateScale(const Vector3& value)
{
	return Matrix3::CreateScale(value.X, value.Y, value.Z);
}

SE_INLINE Matrix3 Matrix3::CreateFromYawPitchRoll(real yaw, real pitch, real roll)
{
	Matrix3 result, matYaw, matPitch, matRoll;

	matYaw = Matrix3::CreateRotationY(yaw);
	matPitch = Matrix3::CreateRotationX(pitch);
	matRoll = Matrix3::CreateRotationZ(roll);
	result = matRoll * matPitch * matYaw;

	return result;
}

SE_INLINE Matrix3 Matrix3::CreateFromAxisAngle(const Vector3& axis, real angle)
{
	Matrix3 result;
	real fSin, fCos, invCos;

	fSin = Math::Sin(angle);
	fCos = Math::Cos(angle);
	invCos = (real)1.0 - fCos;

	result.M00 = invCos * axis.X * axis.X + fCos;
	result.M01 = invCos * axis.X * axis.Y - axis.Z * fSin;
	result.M02 = invCos * axis.X * axis.Z + axis.Y * fSin;

	result.M10 = invCos * axis.X * axis.Y + axis.Z * fSin;
	result.M11 = invCos * axis.Y * axis.Y + fCos;
	result.M12 = invCos * axis.Y * axis.Z - axis.X * fSin;

	result.M20 = invCos * axis.X * axis.Z - axis.Y * fSin;
	result.M21 = invCos * axis.Y * axis.Z + axis.X * fSin;
	result.M22 = invCos * axis.Z * axis.Z + fCos;

	return result;
}

SE_INLINE Matrix3 Matrix3::CreateFromQuaternion(const Quaternion& q)
{
	Matrix3 result;
	real fTx  = (real)2.0*q.X;
	real fTy  = (real)2.0*q.Y;
	real fTz  = (real)2.0*q.Z;
	real fTwx = fTx*q.W;
	real fTwy = fTy*q.W;
	real fTwz = fTz*q.W;
	real fTxx = fTx*q.X;
	real fTxy = fTy*q.X;
	real fTxz = fTz*q.X;
	real fTyy = fTy*q.Y;
	real fTyz = fTz*q.Y;
	real fTzz = fTz*q.Z;

	result.M[0][0] = (real)1.0-(fTyy+fTzz);
	result.M[0][1] = fTxy-fTwz;
	result.M[0][2] = fTxz+fTwy;
	result.M[1][0] = fTxy+fTwz;
	result.M[1][1] = (real)1.0-(fTxx+fTzz);
	result.M[1][2] = fTyz-fTwx;
	result.M[2][0] = fTxz-fTwy;
	result.M[2][1] = fTyz+fTwx;
	result.M[2][2] = (real)1.0-(fTxx+fTyy);

	return result;
}

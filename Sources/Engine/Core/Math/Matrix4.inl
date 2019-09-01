/*=============================================================================
Matrix4.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Matrix4::Matrix4()
{
}

SE_INLINE Matrix4::Matrix4(const Matrix4& value)
{
	Memory::Copy(Data, (void*)&value, sizeof(Matrix4));
}

SE_INLINE Matrix4::Matrix4(
	real m00, real m01, real m02, real m03,
	real m10, real m11, real m12, real m13,
	real m20, real m21, real m22, real m23,
	real m30, real m31, real m32, real m33)
{
	M00 = m00; M01 = m01; M02 = m02; M03 = m03;
	M10 = m10; M11 = m11; M12 = m12; M13 = m13;
	M20 = m20; M21 = m21; M22 = m22; M23 = m23;
	M30 = m30; M31 = m31; M32 = m32; M33 = m33;
}

SE_INLINE Matrix4::Matrix4(const real value[16])
{
	Memory::Copy(Data, (void*)value, 16 * sizeof(real));
}

SE_INLINE Matrix4::Matrix4(const Matrix3& rotation, const Vector3& translation)
{
	M00 = rotation.M00;
	M01 = rotation.M01;
	M02 = rotation.M02;
	M03 = translation.X;

	M10 = rotation.M10;
	M11 = rotation.M11;
	M12 = rotation.M12;
	M13 = translation.Y;

	M20 = rotation.M20;
	M21 = rotation.M21;
	M22 = rotation.M22;
	M23 = translation.Z;

	M30 = (real)0.0;
	M31 = (real)0.0;
	M32 = (real)0.0;
	M33 = (real)1.0;
}

SE_INLINE real& Matrix4::operator()(int row, int col)
{
	return M[row][col];
}

SE_INLINE real Matrix4::operator()(int row, int col) const
{
	return M[row][col];
}

SE_INLINE Matrix4::operator real*()
{
	return (real*)Data;
}

SE_INLINE Matrix4::operator const real*() const
{
	return (const real*)Data;
}

SE_INLINE Matrix4& Matrix4::operator+=(const Matrix4& value)
{
	*this = (*this + value);
	return *this;
}

SE_INLINE Matrix4& Matrix4::operator-=(const Matrix4& value)
{
	*this = (*this - value);
	return *this;
}

SE_INLINE Matrix4& Matrix4::operator*=(const Matrix4& value)
{
	*this = (*this * value);
	return *this;
}

SE_INLINE Matrix4& Matrix4::operator*=(real value)
{
	M00 *= value;
	M01 *= value;
	M02 *= value;
	M03 *= value;
	
	M10 *= value;
	M11 *= value;
	M12 *= value;
	M13 *= value;

	M20 *= value;
	M21 *= value;
	M22 *= value;
	M23 *= value;

	M30 *= value;
	M31 *= value;
	M32 *= value;
	M33 *= value;

	return *this;
}

SE_INLINE Matrix4& Matrix4::operator/=(real value)
{
	*this = (*this / value);
	return *this;
}

SE_INLINE Matrix4 Matrix4::operator+() const
{
	return *this;
}

SE_INLINE Matrix4 Matrix4::operator-() const
{
	return Matrix4(
		-M00, -M01, -M02, -M03,
		-M10, -M11, -M12, -M13,
		-M20, -M21, -M22, -M23,
		-M30, -M31, -M32, -M33);
}

SE_INLINE Matrix4 Matrix4::operator+(const Matrix4& value) const
{
	return Matrix4(
		M00 + value.M00, M01 + value.M01, M02 + value.M02, M03 + value.M03,
		M10 + value.M10, M11 + value.M11, M12 + value.M12, M13 + value.M13,
		M20 + value.M20, M21 + value.M21, M22 + value.M22, M23 + value.M23,
		M30 + value.M30, M31 + value.M31, M32 + value.M32, M33 + value.M33);
}

SE_INLINE Matrix4 Matrix4::operator-(const Matrix4& value) const
{
	return Matrix4(
		M00 - value.M00, M01 - value.M01, M02 - value.M02, M03 - value.M03,
		M10 - value.M10, M11 - value.M11, M12 - value.M12, M13 - value.M13,
		M20 - value.M20, M21 - value.M21, M22 - value.M22, M23 - value.M23,
		M30 - value.M30, M31 - value.M31, M32 - value.M32, M33 - value.M33);
}

SE_INLINE Matrix4 Matrix4::operator*(const Matrix4& value) const
{
	Matrix4 result;
	int i, j, k;

	for (j = 0;j < 4; j++)
	{
		for (i = 0; i < 4; i++)
		{
			result.M[j][i] = (real)0.0;
			for (k = 0; k < 4; k++)
				result.M[j][i] += M[j][k] * value.M[k][i];
		}
	}

	return result;
}

SE_INLINE Matrix4 Matrix4::operator*(real value) const
{
	return Matrix4(
		M00 * value, M01 * value, M02 * value, M03 * value,
		M10 * value, M11 * value, M12 * value, M13 * value,
		M20 * value, M21 * value, M22 * value, M23 * value,
		M30 * value, M31 * value, M32 * value, M33 * value);
}

SE_INLINE Matrix4 Matrix4::operator/(real value) const
{
	real inv = (real)1.0 / value;
	return Matrix4(
		M00 * inv, M01 * inv, M02 * inv, M03 * inv,
		M10 * inv, M11 * inv, M12 * inv, M13 * inv,
		M20 * inv, M21 * inv, M22 * inv, M23 * inv,
		M30 * inv, M31 * inv, M32 * inv, M33 * inv);
}

SE_INLINE bool Matrix4::operator==(const Matrix4& value) const
{
	return (Memory::Compare((void*)this, (void*)&value, sizeof(Matrix4)) == 0);
}

SE_INLINE bool Matrix4::operator!=(const Matrix4& value) const
{
	return !(*this == value);
}

SE_INLINE Vector4 Matrix4::GetRow(int row) const
{
	return *(Vector4*)&Data[4*row];
}

SE_INLINE void Matrix4::SetRow(int row, const Vector4& value)
{
	M[row][0] = value.X;
	M[row][1] = value.Y;
	M[row][2] = value.Z;
	M[row][3] = value.W;
}

SE_INLINE Vector4 Matrix4::GetColumn(int column) const
{
	return Vector4(M[0][column], M[1][column], M[2][column], M[3][column]);
}

SE_INLINE void Matrix4::SetColumn(int column, const Vector4& value)
{
	M[0][column] = value.X;
	M[1][column] = value.Y;
	M[2][column] = value.Z;
	M[3][column] = value.W;
}

SE_INLINE Vector3 Matrix4::GetTranslation() const
{
	return GetColumn(3);
}

SE_INLINE void Matrix4::SetTranslation(const Vector3& value)
{
	SetColumn(3, value);
}

SE_INLINE void Matrix4::SetIdentity()
{
	M[0][1] = M[0][2] = M[0][3] =
	M[1][0] = M[1][2] = M[1][3] =
	M[2][0] = M[2][1] = M[2][3] =
	M[3][0] = M[3][1] = M[3][2] = (real)0.0;
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = (real)1.0;
}

SE_INLINE bool Matrix4::IsIdentity() const
{
	return (
		M[0][0] == (real)1.0 && M[0][1] == (real)0.0 && M[0][2] == (real)0.0 && M[0][3] == (real)0.0 &&
		M[1][0] == (real)0.0 && M[1][1] == (real)1.0 && M[1][2] == (real)0.0 && M[1][3] == (real)0.0 &&
		M[2][0] == (real)0.0 && M[2][1] == (real)0.0 && M[2][2] == (real)1.0 && M[2][3] == (real)0.0 &&
		M[3][0] == (real)0.0 && M[3][1] == (real)0.0 && M[3][2] == (real)0.0 && M[3][3] == (real)1.0);
}

SE_INLINE void Matrix4::Transpose()
{
	*this = Matrix4::Transpose(*this);
}

SE_INLINE Matrix4 Matrix4::Transpose(const Matrix4& value)
{
	Matrix4 result;
	int i, j;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 4; i++)
		{
			result.M[j][i] = value.M[i][j];
		}
	}

	return result;
}

SE_INLINE void Matrix4::Add(const Matrix4& value)
{
	*this += value;
}

SE_INLINE Matrix4 Matrix4::Add(const Matrix4& left, const Matrix4& right)
{
	return (left + right);
}

SE_INLINE void Matrix4::Substract(const Matrix4& value)
{
	*this -= value;
}

SE_INLINE Matrix4 Matrix4::Substract(const Matrix4& left, const Matrix4& right)
{
	return (left - right);
}

SE_INLINE void Matrix4::Multiply(const Matrix4& mat)
{
	*this = Multiply(*this, mat);
}

SE_INLINE Matrix4 Matrix4::Multiply(const Matrix4& left, const Matrix4& right)
{
	return (left * right);
}

SE_INLINE void Matrix4::MultiplyTranspose(const Matrix4& value)
{
	Multiply(value);
	Transpose();
}

SE_INLINE Matrix4 Matrix4::MultiplyTranspose(const Matrix4& left, const Matrix4& right)
{
	Matrix4 result = Matrix4::Multiply(left, right);
	result.Transpose();
	return result;
}

SE_INLINE void Matrix4::Invert()
{
	*this = Matrix4::Invert(*this);
}

SE_INLINE void Matrix4::Translate(real x, real y, real z)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		M[i][3] += M[i][0] * x + M[i][1] * y + M[i][2] * z;
	}
}

SE_INLINE void Matrix4::Translate(const Vector3& v)
{
	Translate(v.X, v.Y, v.Z);
}

SE_INLINE void Matrix4::Scale(real x, real y, real z)
{
	int i;

	for (i = 0; i < 4; i++)
	{
		M[0][i] *= x;
		M[1][i] *= y;
		M[2][i] *= z;
	}
}

SE_INLINE void Matrix4::Scale(const Vector3& value)
{
	Scale(value.X, value.Y, value.Z);
}

SE_INLINE void Matrix4::RotateX(real angle)
{
	(*this) *= Matrix4::CreateRotationX(angle);
}

SE_INLINE void Matrix4::RotateY(real angle)
{
	(*this) *= Matrix4::CreateRotationY(angle);
}

SE_INLINE void Matrix4::RotateZ(real angle)
{
	(*this) *= Matrix4::CreateRotationZ(angle);
}

SE_INLINE Matrix4 Matrix4::CreateTranslation(real x, real y, real z)
{
	Matrix4 result;
	result.M[0][0] = (real)1.0;
	result.M[0][1] = (real)0.0;
	result.M[0][2] = (real)0.0;
	result.M[0][3] = x;
	
	result.M[1][0] = (real)0.0;
	result.M[1][1] = (real)1.0;
	result.M[1][2] = (real)0.0;
	result.M[1][3] = y;
	
	result.M[2][0] = (real)0.0;
	result.M[2][1] = (real)0.0;
	result.M[2][2] = (real)1.0;
	result.M[2][3] = z;
	
	result.M[3][0] = (real)0.0;
	result.M[3][1] = (real)0.0;
	result.M[3][2] = (real)0.0;
	result.M[3][3] = (real)1.0;

	return result;
}

SE_INLINE Matrix4 Matrix4::CreateTranslation(const Vector3& value)
{
	return Matrix4::CreateTranslation(value.X, value.Y, value.Z);
}

SE_INLINE Matrix4 Matrix4::CreateRotationX(real angle)
{
	Matrix4 result;
	real sinAngle, cosAngle;

	sinAngle = Math::Sin(angle);
	cosAngle = Math::Cos(angle);

	result.M00 = (real)1.0;
	result.M01 = (real)0.0;
	result.M02 = (real)0.0;
	result.M03 = (real)0.0;
	
	result.M10 = (real)0.0;
	result.M11 = cosAngle;
	result.M12 = -sinAngle;
	result.M13 = (real)0.0;
	
	result.M20 = (real)0.0;
	result.M21 = sinAngle;
	result.M22 = cosAngle;
	result.M23 = (real)0.0;
	
	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)0.0;
	result.M33 = (real)1.0;

	return result;
}

SE_INLINE Matrix4 Matrix4::CreateRotationY(real angle)
{
	Matrix4 result;
	real sinAngle, cosAngle;

	sinAngle = Math::Sin(angle);
	cosAngle = Math::Cos(angle);

	result.M00 = cosAngle;
	result.M01 = (real)0.0;
	result.M02 = sinAngle;
	result.M03 = (real)0.0;
	
	result.M10 = (real)0.0;
	result.M11 = (real)1.0;
	result.M12 = (real)0.0;
	result.M13 = (real)0.0;
	
	result.M20 = -sinAngle;
	result.M21 = (real)0.0;
	result.M22 = cosAngle;
	result.M23 = (real)0.0;
	
	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)0.0;
	result.M33 = (real)1.0;

	return result;
}

SE_INLINE Matrix4 Matrix4::CreateRotationZ(real angle)
{
	Matrix4 result;
	real sinAngle, cosAngle;

	sinAngle = Math::Sin(angle);
	cosAngle = Math::Cos(angle);

	result.M00 = cosAngle;
	result.M01 = -sinAngle;
	result.M02 = (real)0.0;
	result.M03 = (real)0.0;
	
	result.M10 = sinAngle;
	result.M11 = cosAngle;
	result.M12 = (real)0.0;
	result.M13 = (real)0.0;
	
	result.M20 = (real)0.0;
	result.M21 = (real)0.0;
	result.M22 = (real)1.0;
	result.M23 = (real)0.0;
	
	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)0.0;
	result.M33 = (real)1.0;

	return result;
}

SE_INLINE Matrix4 Matrix4::CreateScale(real x, real y, real z)
{
	Matrix4 result;
	result.M[0][0] = x;
	result.M[0][1] = (real)0.0;
	result.M[0][2] = (real)0.0;
	result.M[0][3] = (real)0.0;
	
	result.M[1][0] = (real)0.0;
	result.M[1][1] = y;
	result.M[1][2] = (real)0.0;
	result.M[1][3] = (real)0.0;
	
	result.M[2][0] = (real)0.0;
	result.M[2][1] = (real)0.0;
	result.M[2][2] = z;
	result.M[2][3] = (real)0.0;
	
	result.M[3][0] = (real)0.0;
	result.M[3][1] = (real)0.0;
	result.M[3][2] = (real)0.0;
	result.M[3][3] = (real)1.0;

	return result;
}

SE_INLINE Matrix4 Matrix4::CreateScale(const Vector3& value)
{
	return Matrix4::CreateScale(value.X, value.Y, value.Z);
}

SE_INLINE Matrix4 Matrix4::CreatePerspective(real32 fov, real32 aspect, real32 znear, real32 zfar)
{
#if SE_USE_COORDINATE_LH
	return Matrix4::CreatePerspectiveLH(fov, aspect, znear, zfar);
#else
	return Matrix4::CreatePerspectiveRH(fov, aspect, znear, zfar);
#endif
}

SE_INLINE Matrix4 Matrix4::CreateOrthographic(real32 width, real32 height, real32 znear, real32 zfar)
{
#if SE_USE_COORDINATE_LH
	return Matrix4::CreateOrthographicLH(width, height, znear, zfar);
#else
	return Matrix4::CreateOrthographicRH(width, height, znear, zfar);
#endif
}

SE_INLINE Matrix4 Matrix4::CreateOrthographicOffCenter(real32 left, real32 right, real32 bottom, real32 top, real32 znear, real32 zfar)
{
#if SE_USE_COORDINATE_LH
	return Matrix4::CreateOrthographicOffCenterLH(left, right, bottom, top, znear, zfar);
#else
	return Matrix4::CreateOrthographicOffCenterRH(left, right, bottom, top, znear, zfar);
#endif
}

SE_INLINE Matrix4 Matrix4::CreateFromYawPitchRoll(real yaw, real pitch, real roll)
{
	Matrix4 result;
	Matrix4 matYaw, matPitch, matRoll;

	matYaw = Matrix4::CreateRotationY(yaw);
	matPitch = Matrix4::CreateRotationX(pitch);
	matRoll = Matrix4::CreateRotationZ(roll);
	result = matRoll * matPitch * matYaw;

	return result;
}

SE_INLINE Matrix4 Matrix4::CreateFromAxisAngle(const Vector3& axis, real angle)
{
	Matrix4 result;
	real sinAngle, cosAngle, invCos;

	sinAngle = Math::Sin(angle);
	cosAngle = Math::Cos(angle);
	invCos = (real)1.0 - cosAngle;

	result.M00 = invCos * axis.X * axis.X + cosAngle;
	result.M01 = invCos * axis.X * axis.Y - axis.Z * sinAngle;
	result.M02 = invCos * axis.X * axis.Z + axis.Y * sinAngle;
	result.M03 = (real)0.0;

	result.M10 = invCos * axis.X * axis.Y + axis.Z * sinAngle;
	result.M11 = invCos * axis.Y * axis.Y + cosAngle;
	result.M12 = invCos * axis.Y * axis.Z - axis.X * sinAngle;
	result.M13 = (real)0.0;

	result.M20 = invCos * axis.X * axis.Z - axis.Y * sinAngle;
	result.M21 = invCos * axis.Y * axis.Z + axis.X * sinAngle;
	result.M22 = invCos * axis.Z * axis.Z + cosAngle;
	result.M23 = (real)0.0;

	result.M30 = (real)0.0;
	result.M31 = (real)0.0;
	result.M32 = (real)0.0;
	result.M33 = (real)1.0;

	return result;
}

SE_INLINE Matrix4 Matrix4::CreateFromQuaternion(const Quaternion& value)
{
	Matrix4 result;
	real fTx  = (real)2.0*value.X;
	real fTy  = (real)2.0*value.Y;
	real fTz  = (real)2.0*value.Z;
	real fTwx = fTx*value.W;
	real fTwy = fTy*value.W;
	real fTwz = fTz*value.W;
	real fTxx = fTx*value.X;
	real fTxy = fTy*value.X;
	real fTxz = fTz*value.X;
	real fTyy = fTy*value.Y;
	real fTyz = fTz*value.Y;
	real fTzz = fTz*value.Z;

	result.M[0][0] = (real)1.0-(fTyy+fTzz);
	result.M[0][1] = fTxy-fTwz;
	result.M[0][2] = fTxz+fTwy;
	result.M[0][3] = (real)0.0;
	result.M[1][0] = fTxy+fTwz;
	result.M[1][1] = (real)1.0-(fTxx+fTzz);
	result.M[1][2] = fTyz-fTwx;
	result.M[1][3] = (real)0.0;
	result.M[2][0] = fTxz-fTwy;
	result.M[2][1] = fTyz+fTwx;
	result.M[2][2] = (real)1.0-(fTxx+fTyy);
	result.M[2][3] = (real)0.0;
	result.M[3][0] = (real)0.0;
	result.M[3][1] = (real)0.0;
	result.M[3][2] = (real)0.0;
	result.M[3][3] = (real)1.0;

	return result;
}

/*=============================================================================
Complex.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Complex::Complex()
{
}

SE_INLINE Complex::Complex(real re, real im)
{
	Real = re;
	Imaginary = im;
}

SE_INLINE Complex::Complex(const Complex& value)
{
	Real = value.Real;
	Imaginary = value.Imaginary;
}

SE_INLINE Complex::operator const real*() const
{
	return (const real*)val;
}

SE_INLINE Complex::operator real*()
{
	return (real*)val;
}

SE_INLINE real Complex::operator[](int index) const
{
	assert(0 <= index && index < 2);
	return val[index];
}

SE_INLINE real& Complex::operator[](int index)
{
	assert(0 <= index && index < 2);
	return val[index];
}

SE_INLINE Complex Complex::operator+() const
{
	return *this;
}

SE_INLINE Complex Complex::operator-() const
{
	return Complex(-Real, -Imaginary);
}

SE_INLINE Complex& Complex::operator+=(const Complex& value)
{
	Real += value.Real;
	Imaginary += value.Imaginary;
	return *this;
}

SE_INLINE Complex& Complex::operator-=(const Complex& value)
{
	Real -= value.Real;
	Imaginary -= value.Imaginary;
	return *this;
}

SE_INLINE Complex& Complex::operator*=(const Complex& value)
{
	*this = Complex(
		Real * value.Real - Imaginary * value.Imaginary,
		Real * value.Imaginary + value.Real * Imaginary);
	return *this;
}

SE_INLINE Complex& Complex::operator/=(const Complex& value)
{
	real s, t;
	if (Math::Abs(value.Real) >= Math::Abs(value.Imaginary))
	{
		s = value.Imaginary / value.Real;
		t = (real)1.0 / (value.Real + s * value.Imaginary);
		*this = Complex((Real + s * Imaginary) * t, (Imaginary - s * Real) * t);
	}
	else
	{
		s = value.Real / value.Imaginary;
		t = (real)1.0 / (s * value.Real + value.Imaginary);
		*this = Complex((Real * s + Imaginary) * t, (Imaginary * s - Real) * t);
	}
	return *this;
}

SE_INLINE Complex& Complex::operator+=(real value)
{
	Real += value;
	return *this;
}

SE_INLINE Complex& Complex::operator-=(real value)
{
	Real -= value;
	return *this;
}

SE_INLINE Complex& Complex::operator*=(real value)
{
	Real *= value;
	Imaginary *= value;
	return *this;
}

SE_INLINE Complex& Complex::operator/=(real value)
{
	assert(value);
	real inv = (real)1.0 / value;
	Real *= inv;
	Imaginary *= inv;
	return *this;
}

SE_INLINE Complex Complex::operator+(const Complex& value) const
{
	return Complex(Real + value.Real, Imaginary + value.Imaginary);
}

SE_INLINE Complex Complex::operator-(const Complex& value) const
{
	return Complex(Real - value.Real, Imaginary - value.Imaginary);
}

SE_INLINE Complex Complex::operator*(const Complex& value) const
{
	return Complex(
		Real * value.Real - Imaginary * value.Imaginary,
		Real * value.Imaginary + value.Real * Imaginary);
}

SE_INLINE Complex Complex::operator/(const Complex& value) const
{
	real s, t;
	if (Math::Abs(value.Real) >= Math::Abs(value.Imaginary))
	{
		s = value.Imaginary / value.Real;
		t = (real)1.0 / (value.Real + s * value.Imaginary);
		return Complex((Real + s * Imaginary) * t, (Imaginary - s * Real) * t);
	}
	else
	{
		s = value.Real / value.Imaginary;
		t = (real)1.0 / (s * value.Real + value.Imaginary);
		return Complex((Real * s + Imaginary) * t, (Imaginary * s - Real) * t);
	}
}

SE_INLINE Complex Complex::operator+(real value) const
{
	return Complex(Real + value, Imaginary);
}

SE_INLINE Complex Complex::operator-(real value) const
{
	return Complex(Real - value, Imaginary);
}

SE_INLINE Complex Complex::operator*(real value) const
{
	return Complex(Real * value, Imaginary * value);
}

SE_INLINE Complex Complex::operator/(real value) const
{
	assert(value);
	real inv = (real)1.0 / value;
	return Complex(Real * inv, Imaginary * inv);
}

SE_INLINE Complex operator+(real valA, const Complex& valB)
{
	return Complex(valA + valB.Real, valB.Imaginary);
}

SE_INLINE Complex operator-(real valA, const Complex& valB)
{
	return Complex(valA - valB.Real, -valB.Imaginary);
}

SE_INLINE Complex operator*(real valA, const Complex& valB)
{
	return Complex(valA * valB.Real, valA * valB.Imaginary);
}

SE_INLINE Complex operator/(real valA, const Complex& valB)
{
	real s, t;
	if (Math::Abs(valB.Real) >= Math::Abs(valB.Imaginary))
	{
		s = valB.Imaginary / valB.Real;
		t = valA / (valB.Real + s * valB.Imaginary);
		return Complex(t, -(s * t));
	}
	else
	{
		s = valB.Real / valB.Imaginary;
		t = valA / (s * valB.Real + valB.Imaginary);
		return Complex(s * t, -t);
	}
}

SE_INLINE bool Complex::operator==(const Complex& value) const
{
	return ((Real == value.Real) && (Imaginary == value.Imaginary));
}

SE_INLINE bool Complex::operator!=(const Complex& value) const
{
	return !(*this == value);
}

SE_INLINE real Complex::Abs() const
{
    return Math::Sqrt(Real * Real + Imaginary * Imaginary);
}

SE_INLINE real Complex::Arg() const
{
    if (Real != (real)0.0)
        return ((real)180.0 / Math::Pi) * Math::Atan(Imaginary / Real);
    else
        return (real)0.0;
}

SE_INLINE Complex Complex::Conj() const
{
    return Complex(Real, -Imaginary);
}

SE_INLINE Complex Complex::Exp() const
{
    real realExp = Math::Exp(Real);
    return Complex(
        realExp * Math::Cos(Imaginary),
        realExp * Math::Sin(Imaginary));
}

SE_INLINE Complex Complex::Log() const
{
    real re = Math::Sqrt(Real * Real + Imaginary * Imaginary);
    real im = Math::Atan2(Imaginary, Real);
    if (im > Math::Pi)
        im = im - (real)2.0 * Math::Pi;
    return Complex(Math::Log(re), im);
}

SE_INLINE Complex Complex::Sqrt()
{
    real abs = Math::Sqrt(Real * Real + Imaginary * Imaginary);
    real re = Math::Sqrt((real)0.5 * (abs + Real));
    real im = Math::Sqrt((real)0.5 * (abs - Real));
    if (Imaginary < (real)0.0)
        im = -im;
    return Complex(re, im);
}

SE_INLINE Complex Complex::Pow(const Complex& value) const
{
    return (Log() * value).Exp();
}

SE_INLINE Complex Complex::Pow(real value) const
{
    return (Log() * value).Exp();
}

SE_INLINE Complex Complex::Sin() const
{
    return Complex(
        Math::Sin(Real) * Math::Cosh(Imaginary),
        Math::Cos(Real) * Math::Sinh(Imaginary));
}

SE_INLINE Complex Complex::Cos() const
{
    return Complex(
        Math::Cos(Real) * Math::Cosh(Imaginary),
        -Math::Sin(Real) * Math::Sinh(Imaginary));
}

SE_INLINE Complex Complex::Tan() const
{
    return this->Sin() / this->Cos();
}

SE_INLINE bool Complex::Equals(const Complex& value) const
{
    return (*this == value);
}

SE_INLINE bool Complex::Equals(const Complex& value, real epsilon) const
{
	if (Math::Abs(Real - value.Real) > epsilon)
	{
		return false;
	}
	if (Math::Abs(Imaginary - value.Imaginary) > epsilon)
	{
		return false;
	}
	return true;
}

SE_INLINE int32 Complex::GetHashCode() const
{
	return (RealType(Real).GetHashCode() ^ RealType(Imaginary).GetHashCode());
}

SE_INLINE String Complex::ToString() const
{
	return String::Format(_T("(%f,%f)"), Real, Imaginary);
}

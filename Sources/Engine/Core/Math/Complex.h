/*=============================================================================
Complex.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_COMPLEX_H_
#define _SE_COMPLEX_H_

#include "Core/Common.h"
#include "Core/ValueType.h"
#include "Core/Math/Math.h"

namespace SonataEngine
{

/** Complex number. */
class SE_CORE_EXPORT Complex
{
public:
	/** @name Constructors. */
	//@{
	Complex();
	Complex(real re, real im);
	Complex(const Complex& value);
	//@}

	/** @name Casting Operators. */
	//@{
	/// Retrieves the components as an array of scalars.
	operator const real*() const;
	operator real*();
	//@}

	/** @name Access Operators. */
	//@{
	/// Retrieves a part by its index.
	real operator[](int index) const;
	real& operator[](int index);
	//@}

	/** @name Unary Operators. */
	//@{
	Complex operator+() const;
	Complex operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	/// Adds this instance to another Complex.
	Complex& operator+=(const Complex& value);
	/// Substracts this instance by another Complex.
	Complex& operator-=(const Complex& value);
	/// Multiplies this instance by another Complex.
	Complex& operator*=(const Complex& value);
	/// Divides this instance by another Complex.
	Complex& operator/=(const Complex& value);

	/// Adds this instance to a scalar.
	Complex& operator+=(real value);
	/// Substracts this instance by a scalar.
	Complex& operator-=(real value);
	/// Multiplies this instance by a scalar.
	Complex& operator*=(real value);
	/// Divides this instance by a scalar.
	Complex& operator/=(real value);
	//@}

	/** @name Binary Operators. */
	//@{
	/// Adds two Complex.
	Complex operator+(const Complex& value) const;
	/// Substracts two Complex.
	Complex operator-(const Complex& value) const;
	/// Multiplies a Complex by another Complex.
	Complex operator*(const Complex& value) const;
	/// Divides a Complex by another Complex.
	Complex operator/(const Complex& value) const;

	/// Adds this instance to a scalar.
	Complex operator+(real value) const;
	/// Substracts this instance by a scalar.
	Complex operator-(real value) const;
	/// Multiplies this instance by a scalar.
	Complex operator*(real value) const;
	/// Divides this instance by a scalar.
	Complex operator/(real value) const;

	/// Adds a scalar to a Complex.
	friend Complex operator+(real valA, const Complex& valB);
	/// Substracts a scalar by a Complex.
	friend Complex operator-(real valA, const Complex& valB);
	/// Multiplies a scalar by a Complex.
	friend Complex operator*(real valA, const Complex& valB);
	/// Divides a scalar by a Complex.
	friend Complex operator/(real valA, const Complex& valB);

	/// Compares this instance to another instance to determine whether they are the same.
	bool operator==(const Complex& value) const;
	/// Compares this instance to another instance to determine whether they are different.
	bool operator!=(const Complex& value) const;
	//@}

	/** @name Operations. */
	//@{
	real Abs() const;

	real Arg() const;

	Complex Conj() const;

	Complex Exp() const;

	Complex Log() const;

	Complex Sqrt();

	Complex Pow(const Complex& value) const;

	Complex Pow(real value) const;

	Complex Sin() const;

	Complex Cos() const;

	Complex Tan() const;
	//@}

	bool Equals(const Complex& value) const;
	bool Equals(const Complex& value, real epsilon) const;

	int32 GetHashCode() const;

	String ToString() const;

public:
	static const Complex Zero;

	union
	{
		struct
		{
			real Real, Imaginary;        
		};
		real val[2];
	};
};

#include "Complex.inl"

}

#endif

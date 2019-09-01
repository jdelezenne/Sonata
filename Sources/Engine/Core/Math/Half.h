/*=============================================================================
Half.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_HALF_H_
#define _SE_HALF_H_

#include "Core/Common.h"
#include "Math.h"

#define SE_HALF_MIN 5.96046448e-08f // Smallest positive half
#define SE_HALF_NRM_MIN 6.10351562e-05f // Smallest positive normalized half
#define SE_HALF_MAX 65504.0f // Largest positive half
#define SE_HALF_EPSILON 0.00097656f // Smallest positive e for which

namespace SonataEngine
{

/**
	@brief Half.

	16 bit floating point number.
	@remarks From OpenEXR
*/
class SE_CORE_EXPORT Half
{
public:
	/** @name Constructors. */
	//@{
	Half();
	Half(real32 value);
	//@}

	/** @name Casting Operators. */
	//@{
	/// Convertion to float.
	operator real32() const;
	//@}

	/** @name Unary Operators. */
	//@{
	Half operator+() const;
	Half operator-() const;
	//@}

	/** @name Assignment Operators. */
	//@{
	Half& operator=(const Half& value);
	Half& operator=(real32 value);
	/// Adds this instance to another half.
	Half& operator+=(const Half& value);
	/// Substracts this instance by another half.
	Half& operator-=(const Half& value);
	/// Multiplies this instance by another half.
	Half& operator*=(const Half& value);
	/// Divides this instance by another half.
	Half& operator/=(const Half& value);

	/// Adds this instance to a scalar.
	Half& operator+=(real32 value);
	/// Substracts this instance by a scalar.
	Half& operator-=(real32 value);
	/// Multiplies this instance by a scalar.
	Half& operator*=(real32 value);
	/// Divides this instance by a scalar.
	Half& operator/=(real32 value);
	//@}

	/** @name Binary Operators. */
	//@{
	/// Compares this instance to another instance to determine whether they are the same.
	bool operator==(const Half& value) const;
	/// Compares this instance to another instance to determine whether they are different.
	bool operator!=(const Half& value) const;
	//@}

	/** @name Operations. */
	//@{
	/// Round to n-bit precision.
	Half Round(uint32 n) const;
	//@}

private:
	static real32 Overflow();
	static int16 FromInt(int32 value);

protected:
	union uif
	{
		uint32 i;
		real32 f;
	};

	uint16 _Value;

	static const uif _toFloat[1 << 16];
	static const uint16 _eLut[1 << 9];
};

#include "Half.inl"

}

#endif 

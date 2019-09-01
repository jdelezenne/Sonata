/*=============================================================================
Half.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Half.h"

namespace SonataEngine
{

real32 Half::Overflow()
{
	volatile real32 f = 1e10;

	for (int i = 0; i < 10; i++)	
		f *= f; // this will overflow before

	// the for�loop terminates
	return f;
}

int16 Half::FromInt(int32 value)
{
	// Our floating point number, f, is represented by the bit
	// pattern in integer i.  Disassemble that bit pattern into
	// the sign, s, the exponent, e, and the significand, m.
	// Shift s into the position where it will go in in the
	// resulting half number.
	// Adjust e, accounting for the different exponent bias
	// of float and half (127 versus 15).

	register int32 s = (value >> 16) & 0x00008000;
	register int32 e = ((value >> 23) & 0x000000ff) - (127 - 15);
	register int32 m = value & 0x007fffff;

	// Now reassemble s, e and m into a half:
	if (e <= 0)
	{
		if (e < -10)
		{
			// E is less than -10.  The absolute value of f is
			// less than HALF_MIN (f may be a small normalized
			// float, a denormalized float or a zero).
			// We convert f to a half zero.

			return 0;
		}

		// E is between -10 and 0.  F is a normalized float,
		// whose magnitude is less than HALF_NRM_MIN.
		// We convert f to a denormalized half.

		m = (m | 0x00800000) >> (1 - e);

		// Round to nearest, round "0.5" up.
		// Rounding may cause the significand to overflow and make
		// our number normalized.  Because of the way a half's bits
		// are laid out, we don't have to treat this case separately;
		// the code below will handle it correctly.

		if (m &  0x00001000)
			m += 0x00002000;

		// Assemble the half from s, e (zero) and m.
		return (s | (m >> 13));
	}
	else if (e == 0xff - (127 - 15))
	{
		if (m == 0)
		{
			// F is an infinity; convert f to a half
			// infinity with the same sign as f.

			return (s | 0x7c00);
		}
		else
		{
			// F is a NAN; we produce a half NAN that preserves
			// the sign bit and the 10 leftmost bits of the
			// significand of f, with one exception: If the 10
			// leftmost bits are all zero, the NAN would turn 
			// into an infinity, so we have to set at least one
			// bit in the significand.

			m >>= 13;
			return (s | 0x7c00 | m | (m == 0));
		}
	}
	else
	{
		// E is greater than zero.  F is a normalized float.
		// We try to convert f to a normalized half.
		// Round to nearest, round "0.5" up

		if (m &  0x00001000)
		{
			m += 0x00002000;

			if (m & 0x00800000)
			{
				m =  0; // overflow in significand,
				e += 1; // adjust exponent
			}
		}

		// Handle exponent overflow
		if (e > 30)
		{
			Half::Overflow(); // Cause a hardware floating point overflow;
			return (s | 0x7c00); // if this returns, the half becomes an
		} // infinity with the same sign as f.

		// Assemble the half from s, e and m.
		return (s | (e << 10) | (m >> 13));
	}
}

}

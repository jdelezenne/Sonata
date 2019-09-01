/*=============================================================================
Half.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Half::Half()
{
	_Value = 0;
}

SE_INLINE Half::Half(real32 value)
{
    if (value == 0)
    {
		// Common special case - zero.
		_Value = 0;
    }
    else
    {
		// We extract the combined sign and exponent, e, from our
		// real32ing-point number, f.  Then we convert e to the sign
		// and exponent of the half number via a table lookup.

		uif x;

		x.f = value;

		register int e = (x.i >> 23) & 0x000001ff;

		e = _eLut[e];

		if (e)
		{
			// Simple case - round the significand and
			// combine it with the sign and exponent.
			_Value = e + (((x.i & 0x007fffff) + 0x00001000) >> 13);
		}
		else
		{
			// Difficult case.
			_Value = Half::FromInt(x.i);
		}
    }
}

SE_INLINE Half::operator real32() const
{
	return _toFloat[_Value].f;
}

SE_INLINE Half Half::operator+() const
{
	return *this;
}

SE_INLINE Half Half::operator-() const
{
	Half h;
	h._Value = (_Value ^ 0x8000);
	return h;
}

SE_INLINE Half& Half::operator=(const Half& value)
{
	_Value = value._Value;
	return *this;
}

SE_INLINE Half& Half::operator=(real32 value)
{
	*this = Half(value);
	return *this;
}

SE_INLINE Half& Half::operator+=(const Half& value)
{
	*this = Half(real32(*this) + real32(value));
	return *this;
}

SE_INLINE Half& Half::operator-=(const Half& value)
{
	*this = Half(real32(*this) - real32(value));
	return *this;
}

SE_INLINE Half& Half::operator*=(const Half& value)
{
	*this = Half(real32(*this) * real32(value));
	return *this;
}

SE_INLINE Half& Half::operator/=(const Half& value)
{
	*this = Half(real32(*this) / real32(value));
	return *this;
}

SE_INLINE Half& Half::operator+=(real32 value)
{
	*this = Half(real32(*this) + value);
	return *this;
}

SE_INLINE Half& Half::operator-=(real32 value)
{
	*this = Half(real32(*this) - value);
	return *this;
}

SE_INLINE Half& Half::operator*=(real32 value)
{
	*this = Half(real32(*this) * value);
	return *this;
}

SE_INLINE Half& Half::operator/=(real32 value)
{
	*this = Half(real32(*this) / value);
	return *this;
}

SE_INLINE bool Half::operator==(const Half& value) const
{
	return (_Value == value._Value);
}

SE_INLINE bool Half::operator!=(const Half& value) const
{
	return !(*this == value);
}

SE_INLINE Half Half::Round(uint32 n) const
{
	// Parameter check.
	if (n >= 10)
		return *this;

	// Disassemble h into the sign, s,
	// and the combined exponent and significand, e.
	uint16 s = (_Value & 0x8000);
	uint16 e = (_Value & 0x7fff);

	// Round the exponent and significand to the nearest value
	// where ones occur only in the (10-n) most significant bits.
	// Note that the exponent adjusts automatically if rounding
	// up causes the significand to overflow.
	e >>= 9 - n;
	e += e & 1;
	e <<= 9 - n;

	// Check for exponent overflow.
	if (e >= 0x7c00)
	{
		// Overflow occurred -- truncate instead of rounding.
		e = _Value;
		e >>= 10 - n;
		e <<= 10 - n;
	}

	// Put the original sign bit back.
	Half h;
	h._Value = (s | e);

	return h;
}

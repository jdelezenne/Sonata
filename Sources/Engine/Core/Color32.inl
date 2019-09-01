/*=============================================================================
Color32.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Color32::Color32()
{
	R = G = B = A = 0.0f;
}

SE_INLINE Color32::Color32(real32 red, real32 green, real32 blue, real32 alpha)
{
	R = red;
	G = green;
	B = blue;
	A = alpha;
}

SE_INLINE Color32::Color32(uint32 value, bool isRGBA)
{
	if (isRGBA)
	{
		R = (real32)((value>>RGBARedShift)&0xff) / 255.0f;
		G = (real32)((value>>RGBAGreenShift)&0xff) / 255.0f;
		B = (real32)((value>>RGBABlueShift)&0xff) / 255.0f;
		A = (real32)((value>>RGBAAlphaShift)&0xff) / 255.0f;
	}
	else
	{
		R = (real32)((value>>ARGBRedShift)&0xff) / 255.0f;
		G = (real32)((value>>ARGBGreenShift)&0xff) / 255.0f;
		B = (real32)((value>>ARGBBlueShift)&0xff) / 255.0f;
		A = (real32)((value>>ARGBAlphaShift)&0xff) / 255.0f;
	}
}

SE_INLINE Color32::Color32(const real32* value, bool isRGBA)
{
	if (isRGBA)
	{
		R = value[0];
		G = value[1];
		B = value[2];
		A = value[3];
	}
	else
	{
		R = value[1];
		G = value[2];
		B = value[3];
		A = value[0];
	}
}

SE_INLINE Color32::Color32(const Color32& value)
{
	R = value.R;
	G = value.G;
	B = value.B;
	A = value.A;
}

SE_INLINE void Color32::Set(real32 red, real32 green, real32 blue, real32 alpha)
{
	R = red;
	G = green;
	B = blue;
	A = alpha;
}

SE_INLINE Color32& Color32::operator=(const Color32& value)
{
	R = value.R;
	G = value.G;
	B = value.B;
	A = value.A;
	return *this;
}

SE_INLINE Color32 Color32::FromUInt8(uint8 red, uint8 green, uint8 blue, uint8 alpha)
{
	Color32 result;
	result.R = (real32)red / 255.0f;
	result.G = (real32)green / 255.0f;
	result.B = (real32)blue / 255.0f;
	result.A = (real32)alpha / 255.0f;
	return result;
}

SE_INLINE Color32 Color32::FromRGBA(uint32 value)
{
	return Color32(value, true);
}

SE_INLINE Color32 Color32::FromARGB(uint32 value)
{
	return Color32(value, false);
}

SE_INLINE uint32 Color32::ToARGB() const
{
	return (uint32)
		((((uint32(A*255.0f))&0xff)<<ARGBAlphaShift) |
		(((uint32(R*255.0f))&0xff)<<ARGBRedShift) |
		(((uint32(G*255.0f))&0xff)<<ARGBGreenShift) |
		(((uint32(B*255.0f))&0xff)<<ARGBBlueShift));
}


SE_INLINE uint32 Color32::ToRGBA() const
{
	return (uint32)
		((((uint32(R*255.0f))&0xff)<<RGBARedShift) |
		(((uint32(G*255.0f))&0xff)<<RGBAGreenShift) |
		(((uint32(B*255.0f))&0xff)<<RGBABlueShift) |
		(((uint32(A*255.0f))&0xff)<<RGBAAlphaShift));
}

SE_INLINE Color32::operator uint32() const
{
	return ToRGBA();
}

SE_INLINE Color32::operator real32*()
{
	return (real32*)Data;
}

SE_INLINE Color32::operator const real32*() const
{
	return (const real32*)Data;
}

SE_INLINE Color32& Color32::operator+=(const Color32& value)
{
	R += value.R;
	G += value.G;
	B += value.B;
	A += value.A;
	return *this;
}

SE_INLINE Color32& Color32::operator-=(const Color32& value)
{
	R -= value.R;
	G -= value.G;
	B -= value.B;
	A -= value.A;
	return *this;
}

SE_INLINE Color32& Color32::operator*=(real32 value)
{
	R *= value;
	G *= value;
	B *= value;
	A *= value;
	return *this;
}

SE_INLINE Color32& Color32::operator/=(real32 value)
{
	real32 inv = 1.0f / value;
	R *= inv;
	G *= inv;
	B *= inv;
	A *= inv;
	return *this;
}

SE_INLINE Color32 Color32::operator+() const
{
	return *this;
}

SE_INLINE Color32 Color32::operator-() const
{
	return Color32(1.0f - R, 1.0f - G, 1.0f - B, A);
}

SE_INLINE Color32 Color32::operator+(const Color32& value) const
{
	return Color32(R + value.R, G + value.G, B + value.B, A + value.A);
}

SE_INLINE Color32 Color32::operator-(const Color32& value) const
{
	return Color32(R - value.R, G - value.G, B - value.B, A - value.A);
}

SE_INLINE Color32 Color32::operator*(const Color32& value) const
{
	return Color32(R * value.R, G * value.G, B * value.B, A * value.A);
}

SE_INLINE Color32 Color32::operator*(real32 value) const
{
	return Color32(R * value, G * value, B * value, A * value);
}

SE_INLINE Color32 Color32::operator/(real32 value) const
{
	real32 inv = 1.0f / value;
	return Color32(R * inv, G * inv, B * inv, A * inv);
}

SE_INLINE Color32 operator*(real32 left, const Color32& right)
{
	return Color32(left * right.R, left * right.G, left * right.B, left * right.A);
}

SE_INLINE bool Color32::operator==(const Color32& value) const
{
	return ((R == value.R) && (G == value.G) && (B == value.B) && (A == value.A));
}

SE_INLINE bool Color32::operator!=(const Color32& value) const
{
	return !(*this == value);
}

SE_INLINE void Color32::Negative()
{
	*this = Color32::Negative(*this);
}

SE_INLINE Color32 Color32::Negative(const Color32& value)
{
	return Color32(
		1.0f - value.R,
		1.0f - value.G,
		1.0f - value.B,
		value.A);
}

SE_INLINE void Color32::Add(const Color32& value)
{
	*this = Color32::Subtract(*this, value);
}

SE_INLINE Color32 Color32::Add(const Color32& left, const Color32& right)
{
	return Color32(
		left.R + right.R,
		left.G + right.G,
		left.B + right.B,
		left.A + right.A);
}

SE_INLINE void Color32::Subtract(const Color32& value)
{
	*this = Color32::Subtract(*this, value);
}

SE_INLINE Color32 Color32::Subtract(const Color32& left, const Color32& right)
{
	return Color32(
		left.R - right.R,
		left.G - right.G,
		left.B - right.B,
		left.A - right.A);
}

SE_INLINE void Color32::Clamp(real32 min, real32 max)
{
	*this = Color32::Clamp(*this, min, max);
}

SE_INLINE void Color32::Scale(real32 s)
{
	*this = Color32::Scale(*this, s);
}

SE_INLINE Color32 Color32::Scale(const Color32& color, real32 s)
{
	return Color32(
		color.R * s,
		color.G * s,
		color.B * s,
		color.A * s);
}

SE_INLINE Color32 Color32::Modulate(const Color32& left, const Color32& right)
{
	return Color32(
		left.R * right.R,
		left.G * right.G,
		left.B * right.B,
		left.A * right.A);
}

SE_INLINE void Color32::AdjustSaturation(real32 s)
{
	*this = Color32::AdjustSaturation(*this, s);
}

SE_INLINE void Color32::AdjustContrast(real32 c)
{
	*this = Color32::AdjustContrast(*this, c);
}

/*=============================================================================
Color8.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

SE_INLINE Color8::Color8()
{
	R = G = B = A = 0;
}

SE_INLINE Color8::Color8(uint8 red, uint8 green, uint8 blue, uint8 alpha)
{
	R = red;
	G = green;
	B = blue;
	A = alpha;
}

SE_INLINE Color8::Color8(uint32 value, bool isRGBA)
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

SE_INLINE Color8::Color8(const uint8* value, bool isRGBA)
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

SE_INLINE Color8::Color8(const Color8& value)
{
	R = value.R;
	G = value.G;
	B = value.B;
	A = value.A;
}

SE_INLINE void Color8::Set(uint8 red, uint8 green, uint8 blue, uint8 alpha)
{
	R = red;
	G = green;
	B = blue;
	A = alpha;
}

SE_INLINE Color8& Color8::operator=(const Color8& value)
{
	R = value.R;
	G = value.G;
	B = value.B;
	A = value.A;
	return *this;
}

SE_INLINE Color8 Color8::FromReal32(real32 red, real32 green, real32 blue, real32 alpha)
{
	Color8 result;
	result.R = (uint8)(red * 255.0f);
	result.G = (uint8)(green * 255.0f);
	result.B = (uint8)(blue * 255.0f);
	result.A = (uint8)(alpha * 255.0f);
	return result;
}

SE_INLINE Color8 Color8::FromRGBA(uint32 value)
{
	return Color8(value, true);
}

SE_INLINE Color8 Color8::FromARGB(uint32 value)
{
	return Color8(value, false);
}

SE_INLINE uint32 Color8::ToARGB() const
{
	return (uint32)
		((((uint32(A))&0xff)<<ARGBAlphaShift) |
		(((uint32(R))&0xff)<<ARGBRedShift) |
		(((uint32(G))&0xff)<<ARGBGreenShift) |
		(((uint32(B))&0xff)<<ARGBBlueShift));
}


SE_INLINE uint32 Color8::ToRGBA() const
{
	return (uint32)
		((((uint32(R))&0xff)<<RGBARedShift) |
		(((uint32(G))&0xff)<<RGBAGreenShift) |
		(((uint32(B))&0xff)<<RGBABlueShift) |
		(((uint32(A))&0xff)<<RGBAAlphaShift));
}

SE_INLINE Color8::operator uint32() const
{
	return ToRGBA();
}

SE_INLINE Color8::operator uint8*()
{
	return (uint8*)Data;
}

SE_INLINE Color8::operator const uint8*() const
{
	return (const uint8*)Data;
}

SE_INLINE Color8& Color8::operator+=(const Color8& value)
{
	R += value.R;
	G += value.G;
	B += value.B;
	A += value.A;
	return *this;
}

SE_INLINE Color8& Color8::operator-=(const Color8& value)
{
	R -= value.R;
	G -= value.G;
	B -= value.B;
	A -= value.A;
	return *this;
}

SE_INLINE Color8& Color8::operator*=(uint8 value)
{
	R *= value;
	G *= value;
	B *= value;
	A *= value;
	return *this;
}

SE_INLINE Color8& Color8::operator/=(uint8 value)
{
	real32 inv = 1.0f / (real32)value;
	R *= inv;
	G *= inv;
	B *= inv;
	A *= inv;
	return *this;
}

SE_INLINE Color8 Color8::operator+() const
{
	return *this;
}

SE_INLINE Color8 Color8::operator-() const
{
	return Color8(255 - R, 255 - G, 255 - B, A);
}

SE_INLINE Color8 Color8::operator+(const Color8& value) const
{
	return Color8(R + value.R, G + value.G, B + value.B, A + value.A);
}

SE_INLINE Color8 Color8::operator-(const Color8& value) const
{
	return Color8(R - value.R, G - value.G, B - value.B, A - value.A);
}

SE_INLINE Color8 Color8::operator*(uint8 value) const
{
	return Color8(R * value, G * value, B * value, A * value);
}

SE_INLINE Color8 Color8::operator/(uint8 value) const
{
	real32 inv = 1.0f / (real32)value;
	return Color8(R * inv, G * inv, B * inv, A * inv);
}

SE_INLINE Color8 operator*(uint8 left, const Color8& right)
{
	return Color8(left * right.R, left * right.G, left * right.B, left * right.A);
}

SE_INLINE bool Color8::operator==(const Color8& value) const
{
	return ((R == value.R) && (G == value.G) && (B == value.B) && (A == value.A));
}

SE_INLINE bool Color8::operator!=(const Color8& value) const
{
	return !(*this == value);
}

SE_INLINE void Color8::Negative()
{
	*this = Color8::Negative(*this);
}

SE_INLINE Color8 Color8::Negative(const Color8& value)
{
	return Color8(
		255 - value.R,
		255 - value.G,
		255 - value.B,
		value.A);
}

SE_INLINE void Color8::Add(const Color8& value)
{
	*this = Color8::Subtract(*this, value);
}

SE_INLINE Color8 Color8::Add(const Color8& left, const Color8& right)
{
	return Color8(
		left.R + right.R,
		left.G + right.G,
		left.B + right.B,
		left.A + right.A);
}

SE_INLINE void Color8::Subtract(const Color8& value)
{
	*this = Color8::Subtract(*this, value);
}

SE_INLINE Color8 Color8::Subtract(const Color8& left, const Color8& right)
{
	return Color8(
		left.R - right.R,
		left.G - right.G,
		left.B - right.B,
		left.A - right.A);
}

SE_INLINE void Color8::Scale(uint8 s)
{
	*this = Color8::Scale(*this, s);
}

SE_INLINE Color8 Color8::Scale(const Color8& color, uint8 s)
{
	return Color8(
		color.R * s,
		color.G * s,
		color.B * s,
		color.A * s);
}

SE_INLINE Color8 Color8::Modulate(const Color8& left, const Color8& right)
{
	return Color8(
		left.R * right.R,
		left.G * right.G,
		left.B * right.B,
		left.A * right.A);
}

SE_INLINE void Color8::AdjustSaturation(uint8 s)
{
	*this = Color8::AdjustSaturation(*this, s);
}

SE_INLINE void Color8::AdjustContrast(uint8 c)
{
	*this = Color8::AdjustContrast(*this, c);
}

/*=============================================================================
Math.inl
Project: Sonata Engine
Copyval2 (c) 2005
Julien Delezenne
=============================================================================*/

SE_INLINE real Math::ToRadians(real value)
{
	return (value * ((real)Math::Pi / (real)180.0));
}

SE_INLINE real Math::ToDegrees(real value)
{
	return (value * ((real)180.0 / (real)Math::Pi));
}

SE_INLINE bool Math::Equals(real valA, real valB)
{
	return (Math::Abs(valA - valB) < Math::Epsilon);
}

SE_INLINE bool Math::Equals(real valA, real valB, real epsilon)
{
	return (Math::Abs(valA - valB) < epsilon);
}

template <class T>
T Math::Min(T val1, T val2)
{
	return (val1 < val2 ? val1 : val2);
}

template <class T>
T Math::Max(T val1, T val2)
{
	return (val1 > val2 ? val1 : val2);
}

template <class T>
T Math::Clamp(T value, T min, T max)
{
	return ((value < min) ? min : ((value > max) ? max : value));
}

template <class T>
SE_INLINE T Math::Abs(T value)
{
	return abs(value);
}

SE_INLINE int32 Math::Mod(int32 x, int32 y)
{
	return x % y;
}

SE_INLINE uint32 Math::Mod(uint32 x, uint32 y)
{
	return x % y;
}

SE_INLINE real32 Math::Mod(real32 x, real32 y)
{
	return fmodf(x, y);
}

SE_INLINE real64 Math::Mod(real64 x, real64 y)
{
	return fmod(x, y);
}

SE_INLINE real Math::Round(real value)
{
	real r = ceil(value);
	return r - value <= (real)0.5 ? r : r - (real)1.0;
}

template <class SE_CORE_EXPORT T>
SE_INLINE int Math::Sign(T value)
{
	return (value < (T)0 ? -1 : (value > (T)0 ? +1 : 0));
}

SE_INLINE real Math::Ceiling(real value)
{
	return (real)ceil((real)value);
}

SE_INLINE real Math::Floor(real value)
{
	return (real)floor((real)value);
}

SE_INLINE real Math::Pow(real x, real y)
{
	return (real)pow((real)x, (real)y);
}

SE_INLINE real Math::Sqrt(real value)
{
	return (real)sqrt((real)value);
}

SE_INLINE real Math::InverseSqrt(real value)
{
	return (real)1.0 / (real)sqrt((real)value);
}

SE_INLINE real Math::Exp(real value)
{
	return (real)exp((real)value);
}

SE_INLINE real Math::Log(real value)
{
	return (real)log((real)value);
}

SE_INLINE real Math::Log(real value, real logBase)
{
	return (real)(log10((real)value) / log10((real)logBase));
}

SE_INLINE real Math::Log10(real value)
{
	return (real)log10((real)value);
}

SE_INLINE real Math::Cos(real value)
{
	return (real)cos((real)value);
}

SE_INLINE real Math::Cosh(real value)
{
	return (real)cosh((real)value);
}

SE_INLINE real Math::Acos(real value)
{
	return (real)acos((real)value);
}

SE_INLINE real Math::Sin(real value)
{
	return (real)sin((real)value);
}

SE_INLINE real Math::Sinh(real value)
{
	return (real)sinh((real)value);
}

SE_INLINE real Math::Asin(real value)
{
	return (real)asin((real)value);
}

SE_INLINE real Math::Tan(real value)
{
	return (real)tan((real)value);
}

SE_INLINE real Math::Tanh(real value)
{
	return (real)tanh((real)value);
}

SE_INLINE real Math::Atan(real value)
{
	return (real)atan((real)value);
}

SE_INLINE real Math::Atan2(real x, real y)
{
	return (real)atan2((real)x, (real)y);
}

SE_INLINE void Math::SinCos(real value, real& valSin, real& valCos)
{
	valSin = (real)sin((real)value);
	valCos = (real)cos((real)value);
}

SE_INLINE real Math::Barycentric(real value1, real value2, real value3, real amount1, real amount2)
{
	return (value1 + amount1 * (value2 - value1) + amount2 * (value3 - value1));
}

SE_INLINE real Math::Lerp(real value1, real value2, real amount)
{
	return (value1 + (value2 - value1) * amount);
}

SE_INLINE real Math::SmoothStep(real value1, real value2, real amount)
{
	amount = Math::Clamp(amount, (real)0.0, (real)1.0);
	amount = (amount * amount) * (3.0 - (2.0 * amount));
	return value1 + ((value2 - value1) * amount);
}

SE_INLINE real Math::CatmullRom(real value1, real value2, real value3, real value4, real amount)
{
	real amount2 = amount * amount;
	real amount3 = amount * amount2;

	return 0.5 * (
		(2.0 * value2) +
		(-value1 + value3) * amount +
		((2.0 * value1) - (5.0 * value2) + (4.0 * value3) - value4) * amount2 +
		(-value1 + (3.0 * value2) - (3.0 * value3) + value4) * amount3);
}

SE_INLINE real Math::Hermite(real value1, real tangent1, real value2, real tangent2, real amount)
{
	float amount2 = amount * amount;
	float amount3 = amount * amount2;
	float a = ((2.0 * amount3) - (3.0 * amount2)) + 1.0;
	float b = (-2.0 * amount3) + (3.0 * amount2);
	float c = (amount3 - (2.0 * amount2)) + amount;
	float d = amount3 - amount2;
	return (value1 * a) + (value2 * b) + (tangent1 * c) + (tangent2 * d);
}

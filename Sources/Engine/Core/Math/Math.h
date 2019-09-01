/*=============================================================================
Math.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MATH_H_
#define _SE_MATH_H_

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <math.h>

#include "Core/Common.h"
#include "Core/Math/Axes.h"
#include "Core/System/Random/Random.h"

namespace SonataEngine
{

class Vector2;
class Vector3;
class Vector4;
class Matrix3;
class Matrix4;
class Quaternion;
class Ray2;
class Ray3;
class OBB;
class Color8;
class Color32;

/** Mathematical functions. */
class SE_CORE_EXPORT Math
{
public:
	/** @name Constants. */
	//@{
	/** Represents the smallest positive real greater than zero. */
	static const real Epsilon;

	/** Represents the ratio of the circumference of a circle to its diameter,
		specified by the constant, pi. */
	static const real Pi;

	/** Represents the inverse of the value of pi. */
	static const real InvPi;

	/** Represents the value of pi times two. */
	static const real TwoPi;

	/** Represents the value of pi divided by two. */
	static const real PiByTwo;

	/** Represents the value of pi divided by four. */
	static const real PiByFour;

	/** Represents the value of pi divided by 180. */
	static const real PiBy180;

	/** Represents the natural logarithmic base, specified by the constant, e. */
	static const real E;
	
	/** Represents the log base ten of e. */
	static const real Log10E;

	/** Represents the log base two of e. */
	static const real Log2E;
	//@}

	/** @name Conversions. */
	//@{
	/** Converts radians to degrees. */
	static real ToDegrees(real value);

	/** Converts degrees to radians. */
	static real ToRadians(real value);
	//@}

	static bool Equals(real valA, real valB);
	static bool Equals(real valA, real valB, real epsilon);

	/** @name Conversions. */
	//@{
	/** Returns the lowest of two values. */
	template <class T>
	static T Min(T val1, T val2);

	/** Returns the greater of two values. */
	template <class T>
	static T Max(T val1, T val2);

	/** Restricts a value to be within a specified range. */
	template <class T>
	static T Clamp(T value, T min, T max);
	//@}

	/** @name Operations. */
	//@{
	/** Returns the absolute value of a specified number. */
	template <class T>
	static T Abs(T value);

	/** Returns the floating-point remainder of x divided by y. */
	static int32 Mod(int32 x, int32 y);
	static uint32 Mod(uint32 x, uint32 y);
	static real32 Mod(real32 x, real32 y);
	static real64 Mod(real64 x, real64 y);

	/** Returns the number nearest the specified value. */
	static real Round(real value);

	/** Returns a value indicating the sign of a number. */
	template <class SE_CORE_EXPORT T>
	static int Sign(T value);

	/** Returns the smallest whole number greater than or equal to the specified number. */
	static real Ceiling(real value);

	/** Returns the largest whole number less than or equal to the specified number. */
	static real Floor(real value);

	/** Returns a specified number raised to the specified power. */
	static real Pow(real x, real y);

	/** Returns the square root of a specified number. */
	static real Sqrt(real value);

	/** Returns the reciprocal square root of a specified number. */
	static real InverseSqrt(real value);
	//@}

	/** @name Trigonometry. */
	//@{
	/** Returns e raised to the specified power. */
	static real Exp(real value);

	/** Returns the logarithm of a specified number. */
	static real Log(real value);

	/** Returns the logarithm of a specified number in a specified base. */
	static real Log(real value, real logBase);

	/** Returns the base 10 logarithm of a specified number. */
	static real Log10(real value);

	/** Returns the cosine of the specified angle. */
	static real Cos(real value);

	/** Returns the hyperbolic cosine of the specified angle. */
	static real Cosh(real value);

	/** Returns the angle whose cosine is the specified number. */
	static real Acos(real value);

	/** Returns the sine of the specified angle. */
	static real Sin(real value);

	/** Returns the hyperbolic sine of the specified angle. */
	static real Sinh(real value);

	/** Returns the angle whose sine is the specified number. */
	static real Asin(real value);

	/** Returns the tangent of the specified angle. */
	static real Tan(real value);

	/** Returns the hyperbolic tangent of the specified angle. */
	static real Tanh(real value);

	/** Returns the angle whose tangent is the specified number. */
	static real Atan(real value);

	/** Returns the angle whose tangent is the quotient of two specified numbers. */
	static real Atan2(real x, real y);

	/** Returns the sine and the cosine of the specified angle. */
	static void SinCos(real value, real& valSin, real& valCos);
	//@}

	/** @name Random Numbers. */
	//@{
	static int32 RandomInt();

	static real RandomReal();

	static int32 Random(int32 minValue, int32 maxValue);

	static real Random(real minValue, real maxValue);

	static real UnitRandom();

	static real SymmetricRandom();

	static Vector2 Random(const Vector2& minValue, const Vector2& maxValue);

	static Vector3 Random(const Vector3& minValue, const Vector3& maxValue);

	static Vector4 Random(const Vector4& minValue, const Vector4& maxValue);

	static Color8 Random(const Color8& minValue, const Color8& maxValue);

	static Color32 Random(const Color32& minValue, const Color32& maxValue);
	//@}

	/** @name Interpolations. */
	//@{
	/** Returns a point in barycentric coordinates, using the specified values. */
	static real Barycentric(real value1, real value2, real value3, real amount1, real amount2);

	/**
		Performs a linear interpolation between two values.
		@remarks
			value1 + (value2 - value1) * amount
	*/
	static real Lerp(real value1, real value2, real amount);

	/**
		Performs an interpolation between two vectors using a cubic equation.
		@remarks
			amount = amount^2 * (3 - (2 * amount))
			value1 + ((value2 - value1) * amount)
	*/
	static real SmoothStep(real value1, real value2, real amount);

	/**
		Performs a Catmull-Rom interpolation using the specified positions.
		@remarks
						[-1  3 -3  1] [value1]
			[t3 t2 t 1] [ 3 -6  3  0] [value2]
						[-3  3  0  0] [value3]
						[ 1  0  0  0] [value4]
	*/
	static real CatmullRom(real value1, real value2, real value3, real value4, real amount);

	/**
		Performs a Hermite spline interpolation between two values using the specified tangents.
		@remarks
		                [ 2 -2  1  1] [value1]
			[t3 t2 t 1] [-3  3 -2 -1] [tangent1]
						[ 0  0  1  0] [value2]
						[ 1  0  0  0] [tangent2]
	*/
	static real Hermite(real value1, real tangent1, real value2, real tangent2, real amount);
	//@}

private:
	static SonataEngine::Random& _Random;
};

#include "Math.inl"

}

#endif

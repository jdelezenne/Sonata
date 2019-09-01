/*=============================================================================
Math.cpp
Project: Sonata Engine
Copyright (c) 2005
Julien Delezenne
=============================================================================*/

#include "Math.h"
#include "Core/Common.h"
#include "Core/System/Random/RandomStd.h"
#include "Core/Color8.h"
#include "Core/Color32.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Ray2.h"
#include "Core/Math/Ray3.h"
#include "Core/Math/Plane.h"
#include "Core/Math/Interpolator.h"
#include "Core/Math/ODEsolver.h"

namespace SonataEngine
{

const real Math::Epsilon = 1e-10;
const real Math::Pi = 3.14159265358979323846;
const real Math::InvPi = 0.31830988618379067154;
const real Math::TwoPi = 6.28318530717958647693;
const real Math::PiByTwo = 1.57079632679489661923;
const real Math::PiByFour = 0.78539816339744830962;
const real Math::PiBy180 = 0.01745329251994329576;
const real Math::E = 2.7182818284590452354;
const real Math::Log10E = 0.4342945;
const real Math::Log2E = 1.442695;

Random& Math::_Random = RandomStd();

int32 Math::RandomInt()
{
	return _Random.RandomInt();
}

real Math::RandomReal()
{
	return _Random.RandomReal();
}

int Math::Random(int32 minValue, int32 maxValue)
{
	return _Random.RandomInt(minValue, maxValue);
}

real Math::Random(real minValue, real maxValue)
{
	return _Random.RandomReal(minValue, maxValue);
}

real Math::UnitRandom()
{
	return _Random.RandomReal(0.0f, 1.0f);
}

real Math::SymmetricRandom()
{
	return _Random.RandomReal(-1.0f, 1.0f);
}

Vector2 Math::Random(const Vector2& min, const Vector2& max)
{
	return Vector2(
		_Random.RandomReal(min.X, max.X),
		_Random.RandomReal(min.Y, max.Y)
	);
}

Vector3 Math::Random(const Vector3& min, const Vector3& max)
{
	return Vector3(
		_Random.RandomReal(min.X, max.X),
		_Random.RandomReal(min.Y, max.Y),
		_Random.RandomReal(min.Z, max.Z)
	);
}

Vector4 Math::Random(const Vector4& min, const Vector4& max)
{
	return Vector4(
		_Random.RandomReal(min.X, max.X),
		_Random.RandomReal(min.Y, max.Y),
		_Random.RandomReal(min.Z, max.Z),
		_Random.RandomReal(min.W, max.W)
	);
}

Color8 Math::Random(const Color8& min, const Color8& max)
{
	return Color8(
		_Random.RandomInt(min.R, max.R),
		_Random.RandomInt(min.G, max.G),
		_Random.RandomInt(min.B, max.B),
		_Random.RandomInt(min.A, max.A)
	);
}

Color32 Math::Random(const Color32& min, const Color32& max)
{
	return Color32(
		_Random.RandomReal(min.R, max.R),
		_Random.RandomReal(min.G, max.G),
		_Random.RandomReal(min.B, max.B),
		_Random.RandomReal(min.A, max.A)
	);
}

}

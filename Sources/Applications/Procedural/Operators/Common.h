/*=============================================================================
Common.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _PROCEDURAL_OPERATORS_COMMON_H_
#define _PROCEDURAL_OPERATORS_COMMON_H_

#include "Operators.h"

#define DECLARE_COMPONENTPROPERTY(name, type, canlink, value) \
	{ \
		OperatorProperty* property = new OperatorProperty(this); \
		property->SetName(#name); \
		property->SetType(typeof(type)); \
		property->SetCanLink(false); \
		property->SetValue(ToVariant(value)); \
		_Properties.Add(property); \
	}

#define DECLARE_COMPONENTSLOT(name, type) \
	{ \
		OperatorSlot* slot = new OperatorSlot(this); \
		slot->SetName(#name); \
		slot->SetType(typeof(type)); \
		_Slots.Add(slot); \
	}

#define RETURN_COLOURSLOT(name, value) \
	Slot(name)->SetValue(ToVariant(Color32::Clamp(value, 0.0f, 1.0f)));

namespace Operators
{
	const real32 MinFilterWidth = 1.0e-6;

	SE_INLINE real32 step(real32 edge, real32 x)
	{
		return (real32)(x >= edge);
	}

	SE_INLINE real32 smoothstep(real32 edge0, real32 edge1, real32 x)
	{
		if (x < edge0)
			return 0;
		if (x >= edge1)
			return 1;

		x = (x - edge0) / (edge1 - edge0);
		return (x*x * (3 - 2*x));
	}

	SE_INLINE real32 pulse(real32 edge0, real32 edge1, real32 x)
	{
		return step(edge0, x) - step(edge1, x);
	}

	SE_INLINE real32 pulsetrain (real32 edge, real32 period, real32 x)
	{
		return pulse(edge, period, Math::Mod(x, period));
	}

	SE_INLINE real32 smoothpulse(real32 e0, real32 e1, real32 e2, real32 e3, real32 x)
	{
		return smoothstep(e0, e1, x) - smoothstep(e2, e3, x);
	}

	SE_INLINE real32 smoothpulsetrain(real32 e0, real32 e1, real32 e2, real32 e3, real32 period, real32 x)
	{
		return smoothpulse(e0, e1, e2, e3, Math::Mod(x, period));
	}

	SE_INLINE real32 bias(real32 x, real32 b)
	{
		return Math::Pow(x, Math::Log(b) / Math::Log(0.5));
	}

	SE_INLINE real32 biasperlin(real32 x, real32 b, real32 min, real32 max)
	{
		return Math::Pow(x, Math::Log(Math::Clamp(b, min, max)) / Math::Log(0.5));
	}

	SE_INLINE real32 gain(real32 x, real32 g)
	{
		return ((x < 0.5) ? bias(2*x, 1-g) / 2 : 1-bias(2-2*x, 1-g) / 2);
	}

	/*SE_INLINE real32 square(real32 dir, real32 high, real32 frequency, real32 offset, real32 cycle, real32 width, real32 filterkernel)
	{
		return (high * filterstep((cycle), 1.0 - Math::Abs(2.0 * Math::Mod((dir) * (frequency) + (offset), 1.0) - 1.0),width,filter_kernel));
	}*/

	SE_INLINE real32 triangle(real32 dir, real32 high, real32 frequency, real32 offset)
	{
		return (high * (1.0f - Math::Abs(2.0f * Math::Mod((dir) * (frequency) + (offset), 1.0f) - 1.0f)));
	}

	SE_INLINE Color32 mix(const Color32& c0, const Color32& c1, real32 f)
	{
		return ((1-f)*c0 + f*c1);
	}

	SE_INLINE real32 blendwidth(real32 x, real32 y)
	{
		return (Math::Floor(x)*(1.0f-2.0f*y) + Math::Max(0.0f, Math::Mod(x, 1.0f)-y));
	}

	SE_INLINE real32 filterwidth(real32 x)
	{
		//return (Math::Max((Math::Abs(Du(x)*du)+Math::Abs(Dv(x)*dv)), MinFilterWidth));
		return Math::Floor(x);
	}

	SE_INLINE real32 sq(real32 x)
	{
		return (x * x);
	}
	SE_INLINE real32 cube(real32 x)
	{
		return (x * x * x);
	}

	SE_INLINE real32 quad(real32 x)
	{
		return (x * x * x * x);
	}
	SE_INLINE real32 shift(real32 x)
	{
		return ((x+1) / 2);
	}

	SE_INLINE real32 radius(real32 a, real32 b)
	{
		return Math::Sqrt(a*a + b*b);
	}

	real32 noise1(int32 x);
	real32 noise2(int32 x, int32 y);
	real32 noise3(int32 x, int32 y, int32 z);

	real64 noise(real64 x);
	real64 noise(real64 x, real64 y);
	real64 noise(real64 x, real64 y, real64 z);

	real64 PerlinNoise1D(real64 x, real64 weight, real64 frequence, int32 octaves);
	real64 PerlinNoise2D(real64 x, real64 y, real64 weight, real64 frequence, int32 octaves);
	real64 PerlinNoise3D(real64 x, real64 y, real64 z, real64 weight, real64 frequence, int32 octaves);

	real32 CellNoise(real32 x, real32 y, real32 z);

	real32 SCellNoise(real32 x, real32 y, real32 z);

	real32 DistanceSquared(real32 x, real32 y, real32 z, real32 e);

	real32 DistanceReal(real32 x, real32 y, real32 z, real32 e);

	real32 DistanceManhattan(real32 x, real32 y, real32 z, real32 e);

	real32 DistanceChebychev(real32 x, real32 y, real32 z, real32 e);

	real32 DistanceMinkovskyH(real32 x, real32 y, real32 z, real32 e);

	real32 DistanceMinkovsky4(real32 x, real32 y, real32 z, real32 e);

	real32 DistanceMinkovsky(real32 x, real32 y, real32 z, real32 e);

	void Voronoi_f1_3d(real32 x, real32 y, real32 z, real32 jitter, real32& f1, real32* pos1);
}

#endif 

/*=============================================================================
Common.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "Common.h"

namespace Operators
{
	real32 noise1(int32 x)
	{
		x = (x<<13) ^ x;
		return (real32)(1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) &
			0x7fffffff) / 1073741824.0);
	}

	real32 noise2(int32 x, int32 y)
	{
		int32 n;
		n = x + y * 57;
		n = (n<<13) ^ n;
		return (real32)(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) &
			0x7fffffff) / 1073741824.0);
	}

	real32 noise3(int32 x, int32 y, int32 z)
	{
		return 0.0;
	}

	#define B 0x100
	#define BM 0xff
	#define N 0x1000
	#define NP 12   // 2^N
	#define NM 0xfff

	#define s_curve(t) ( t * t * (3. - 2. * t) )
	#define lerp(t, a, b) ( a + t * (b - a) )
	#define setup(i, b0, b1, r0, r1)\
			t = vec[i] + N;\
			b0 = ((int)t) & BM;\
			b1 = (b0+1) & BM;\
			r0 = t - (int)t;\
			r1 = r0 - 1.;
	#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
	#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	static int32 p[B + B + 2];
	static real64 g3[B + B + 2][3];
	static real64 g2[B + B + 2][2];
	static real64 g1[B + B + 2];
	static int32 start = 1;

	void normalize(real64& x, real64& y)
	{
		real64 s;

		s = Math::Sqrt(x * x + y * y);
		x = x / s;
		y = y / s;
	}

	void normalize(real64& x, real64& y, real64& z)
	{
		real64 s;

		s = Math::Sqrt(x * x + y * y + z * z);
		x = x / s;
		y = y / s;
		z = z / s;
	}

	void init(void)
	{
		int32 i, j, k;

		for (i = 0; i < B; i++)
		{
			p[i] = i;
			g1[i] = (real64)((Math::RandomInt() % (B + B)) - B) / B;

			for (j = 0; j < 2; j++)
				g2[i][j] = (real64)((Math::RandomInt() % (B + B)) - B) / B;
			normalize(g2[i][0], g2[i][1]);

			for (j = 0; j < 3; j++)
				g3[i][j] = (real64)((Math::RandomInt() % (B + B)) - B) / B;
			normalize(g3[i][0], g3[i][1], g3[i][2]);
		}

		while (--i)
		{
			k = p[i];
			p[i] = p[j = Math::RandomInt() % B];
			p[j] = k;
		}

		for (i = 0; i < B + 2; i++)
		{
			p[B + i] = p[i];
			g1[B + i] = g1[i];
			for (j = 0; j < 2; j++)
				g2[B + i][j] = g2[i][j];
			for (j = 0; j < 3; j++)
				g3[B + i][j] = g3[i][j];
		}
	}

	real64 noise(real64 x)
	{
		int32 bx0, bx1;
		real64 rx0, rx1, sx, t, u, v, vec[1];

		vec[0] = x;
		if (start)
		{
			start = 0;
			init();
		}

		setup(0, bx0, bx1, rx0, rx1);

		sx = s_curve(rx0);
		u = rx0 * g1[ p[ bx0 ] ];
		v = rx1 * g1[ p[ bx1 ] ];

		return (lerp(sx, u, v));
	}

	real64 noise(real64 x, real64 y)
	{
		int32 bx0, bx1, by0, by1, b00, b10, b01, b11;
		real64 rx0, rx1, ry0, ry1, *q, sx, sy, a, b, t, u, v, vec[2];
		int32 i, j;

		vec[0] = x;
		vec[1] = y;
		if (start)
		{
			start = 0;
			init();
		}

		setup(0, bx0, bx1, rx0, rx1);
		setup(1, by0, by1, ry0, ry1);

		i = p[ bx0 ];
		j = p[ bx1 ];

		b00 = p[ i + by0 ];
		b10 = p[ j + by0 ];
		b01 = p[ i + by1 ];
		b11 = p[ j + by1 ];

		sx = s_curve(rx0);
		sy = s_curve(ry0);

		q = g2[ b00 ]; u = at2(rx0, ry0);
		q = g2[ b10 ]; v = at2(rx1, ry0);
		a = lerp(sx, u, v);

		q = g2[ b01 ]; u = at2(rx0, ry1);
		q = g2[ b11 ]; v = at2(rx1, ry1);
		b = lerp(sx, u, v);

		return lerp(sy, a, b);
	}

	real64 noise(real64 x, real64 y, real64 z)
	{
		int32 bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
		real64 rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v, vec[3];
		int32 i, j;

		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		if (start)
		{
			start = 0;
			init();
		}

		setup(0, bx0, bx1, rx0, rx1);
		setup(1, by0, by1, ry0, ry1);
		setup(2, bz0, bz1, rz0, rz1);

		i = p[ bx0 ];
		j = p[ bx1 ];

		b00 = p[ i + by0 ];
		b10 = p[ j + by0 ];
		b01 = p[ i + by1 ];
		b11 = p[ j + by1 ];

		t  = s_curve(rx0);
		sy = s_curve(ry0);
		sz = s_curve(rz0);

		q = g3[ b00 + bz0 ]; u = at3(rx0,ry0,rz0);
		q = g3[ b10 + bz0 ]; v = at3(rx1,ry0,rz0);
		a = lerp(t, u, v);

		q = g3[ b01 + bz0 ]; u = at3(rx0,ry1,rz0);
		q = g3[ b11 + bz0 ]; v = at3(rx1,ry1,rz0);
		b = lerp(t, u, v);

		c = lerp(sy, a, b);

		q = g3[ b00 + bz1 ]; u = at3(rx0,ry0,rz1);
		q = g3[ b10 + bz1 ]; v = at3(rx1,ry0,rz1);
		a = lerp(t, u, v);

		q = g3[ b01 + bz1 ]; u = at3(rx0,ry1,rz1);
		q = g3[ b11 + bz1 ]; v = at3(rx1,ry1,rz1);
		b = lerp(t, u, v);

		d = lerp(sy, a, b);

		return lerp(sz, c, d);
	}

	real64 PerlinNoise1D(real64 x, real64 weight, real64 frequence, int32 octaves)
	{
		int32 i;
		real64 val, sum = 0;
		real64 p, scale = 1;

		p = x;
		for (i=0; i<octaves; i++)
		{
			val = noise(p);
			sum += val / scale;
			scale *= weight;
			p *= frequence;
		}
		return sum;
	}

	real64 PerlinNoise2D(real64 x, real64 y, real64 weight, real64 frequence, int32 octaves)
	{
		int32 i;
		real64 val, sum = 0;
		real64 p[2], scale = 1;

		p[0] = x;
		p[1] = y;
		for (i=0; i<octaves; i++)
		{
			val = noise(p[0], p[1]);
			sum += val / scale;
			scale *= weight;
			p[0] *= frequence;
			p[1] *= frequence;
		}
		return sum;
	}

	real64 PerlinNoise3D(real64 x, real64 y, real64 z, real64 weight, real64 frequence, int32 octaves)
	{
		int32 i;
		real64 val, sum = 0;
		real64 p[3], scale = 1;

		p[0] = x;
		p[1] = y;
		p[2] = z;
		for (i=0; i<octaves; i++)
		{
			val = noise(p[0], p[1], p[2]);
			sum += val / scale;
			scale *= weight;
			p[0] *= frequence;
			p[1] *= frequence;
			p[2] *= frequence;
		}
		return sum;
	}

	real32 CellNoise(real32 x, real32 y, real32 z)
	{
		int32 xi = (int32)(Math::Floor(x));
		int32 yi = (int32)(Math::Floor(y));
		int32 zi = (int32)(Math::Floor(z));
		uint32 n = xi + yi*1301 + zi*314159;
		n ^= (n<<13);
		return ((real32)(n*(n*n*15731 + 789221) + 1376312589) / 4294967296.0);
	}

	real32 SCellNoise(real32 x, real32 y, real32 z)
	{
		return (2.0f*CellNoise(x, y, z)-1.0f);
	}

	real32 DistanceSquared(real32 x, real32 y, real32 z, real32 e)
	{
		return (x*x + y*y + z*z);
	}

	real32 DistanceReal(real32 x, real32 y, real32 z, real32 e)
	{
		return Math::Sqrt(x*x + y*y + z*z);
	}

	real32 DistanceManhattan(real32 x, real32 y, real32 z, real32 e)
	{
		return (Math::Abs(x) + Math::Abs(y) + Math::Abs(z));
	}

	real32 DistanceChebychev(real32 x, real32 y, real32 z, real32 e)
	{
		real32 t;
		x = Math::Abs(x);
		y = Math::Abs(y);
		z = Math::Abs(z);
		t = (x > y) ? x : y;
		return ((z>t)?z:t);
	}

	real32 DistanceMinkovskyH(real32 x, real32 y, real32 z, real32 e)
	{
		real32 d = Math::Sqrt(Math::Abs(x)) + Math::Sqrt(Math::Abs(y)) + Math::Sqrt(Math::Abs(z));
		return (d*d);
	}

	real32 DistanceMinkovsky4(real32 x, real32 y, real32 z, real32 e)
	{
		x *= x;
		y *= y;
		z *= z;
		return Math::Sqrt(Math::Sqrt(x*x + y*y + z*z));
	}

	real32 DistanceMinkovsky(real32 x, real32 y, real32 z, real32 e)
	{
		return Math::Pow(Math::Pow(Math::Abs(x), e) + Math::Pow(Math::Abs(y), e) + Math::Pow(Math::Abs(z), e), 1.0/e);
	}

	typedef real32(*fnDistance)(real32,real32,real32,real32);

	void Voronoi_f1_3d(real32 x, real32 y, real32 z, real32 jitter, real32& f1, real32* pos1)
	{
		real32 i, j, k;
		real32 xx, yy, zz;
		real32 tx, ty, tz;
		real32 px, py, pz;
		real32 ox, oy, oz;
		real32 r;

		xx = Math::Floor(x)+0.5;
		yy = Math::Floor(y)+0.5;
		zz = Math::Floor(z)+0.5;
		f1 = 1000;
		for (i = -1;  i <= 1;  i += 1)
		{
			for (j = -1;  j <= 1;  j += 1)
			{
				for (k = -1;  k <= 1;  k += 1)
				{
					tx = xx + i;
					ty = yy + j;
					tz = zz + k;
					r = CellNoise(tx, ty, tz) - 0.5;
					px = tx + jitter * r;
					py = ty + jitter * r;
					pz = tz + jitter * r;
					ox = px - xx;
					oy = py - yy;
					oz = pz - zz;
					real32 dist = DistanceSquared(ox, oy, oz, jitter);
					if (dist < f1)
					{
						f1 = dist;
						pos1[0] = px;
						pos1[1] = py;
						pos1[2] = pz;
					}
				}
			}
		}
		f1 = Math::Sqrt(f1);
	}
}

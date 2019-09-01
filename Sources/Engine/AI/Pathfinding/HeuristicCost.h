/*=============================================================================
HeuristicCost.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_HEURISTICCOST_H_
#define _SE_AI_HEURISTICCOST_H_

#include "AI/Common.h"

namespace SonataEngine
{
	namespace AI
	{
		/** Base class for the distance calculations. */
		template <class T>
		class Distance
		{
		};

		class Distance2 : public Distance<Vector2>
		{
		};

		class Distance3 : public Distance<Vector3>
		{
		};

		/** Chebyshev distance in 2D.
			In mathematics, the Chebyshev distance between two points p and q
			in an Euclidean space with standard coordinates pi and qi respectively is
			maxi( | pi - qi | ).
		*/
		class ChebyshevDistance2 : public Distance2
		{
		public:
			static real Compute(const Vector2& source, const Vector2& destination)
			{
				Vector2 diff = destination - source;
				return Math::Max(Math::Abs(diff.X), Math::Abs(diff.Y));
			}
		};

		/** Chebyshev distance in 3D.
			In mathematics, the Chebyshev distance between two points p and q
			in an Euclidean space with standard coordinates pi and qi respectively is
			maxi( | pi - qi | ).
		*/
		class ChebyshevDistance3 : public Distance3
		{
		public:
			static real Compute(const Vector3& source, const Vector3& destination)
			{
				Vector3 diff = destination - source;
				return Math::Max(Math::Abs(diff.X), Math::Max(Math::Abs(diff.Y), Math::Abs(diff.Z)));
			}
		};

		/** Euclidian distance in 2D. */
		class EuclidianDistance2 : public Distance2
		{
		public:
			static real Compute(const Vector2& source, const Vector2& destination)
			{
				Vector2 diff = destination - source;
				return Math::Sqrt(diff.X * diff.X + diff.Y * diff.Y);
			}
		};

		/** Euclidian distance in 3D. */
		class EuclidianDistance3 : public Distance3
		{
		public:
			static real Compute(const Vector3& source, const Vector3& destination)
			{
				Vector3 diff = destination - source;
				return Math::Sqrt(diff.X * diff.X + diff.Y * diff.Y + diff.Z * diff.Z);
			}
		};

		/** Manhattan distance in 2D.
			The Manhattan distance, also known as the L1-distance, between two points
			in an Euclidean space with fixed Cartesian coordinate system is defined as
			the sum of the lengths of the projections of the line segment between
			the points onto the coordinate axes.
			| dx | + | dy |
		*/
		class ManhattanDistance2 : public Distance2
		{
		public:
			static real Compute(const Vector2& source, const Vector2& destination)
			{
				Vector2 diff = destination - source;
				return Math::Abs(diff.X) + Math::Abs(diff.Y);
			}
		};

		/** Manhattan distance in 2D.
			The Manhattan distance, also known as the L1-distance, between two points
			in an Euclidean space with fixed Cartesian coordinate system is defined as
			the sum of the lengths of the projections of the line segment between
			the points onto the coordinate axes.
			| dx | + | dy | + | dz |
		*/
		class ManhattanDistance3 : public Distance3
		{
		public:
			static real Compute(const Vector3& source, const Vector3& destination)
			{
				Vector3 diff = destination - source;
				return Math::Abs(diff.X) + Math::Abs(diff.Y) + Math::Abs(diff.Z);
			}
		};
	}
}

#endif 

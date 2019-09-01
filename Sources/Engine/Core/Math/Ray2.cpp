/*=============================================================================
Ray2.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Ray2.h"

namespace SonataEngine
{

real Ray2::GetSquareDistance(const Vector2& value, real* t) const
{
	Vector2 diff = value - Origin;
	real ft = Vector2::Dot(Direction, diff);
	if (ft > (real)0.0)
	{
		diff -= ft * Direction;
	}
	else
	{
		ft = (real)0.0;
	}

	if (t != NULL)
		*t = ft;

	return diff.LengthSquared();
}

}

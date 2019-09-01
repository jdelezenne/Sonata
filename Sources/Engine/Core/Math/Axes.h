/*=============================================================================
Axes.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AXES_H_
#define _SE_AXES_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** 3D Axes. */
class SE_CORE_EXPORT Axes
{
public:
	/** Axes. */
	enum Type
	{
		XAxis = 0,
		YAxis = 1,
		ZAxis = 2,
		WAxis = 3
	};

	/** Axes Order. */
	enum Order
	{
		Order_XYZ = (XAxis)|(YAxis<<2)|(ZAxis<<4),
		Order_XZY = (XAxis)|(ZAxis<<2)|(YAxis<<4),
		Order_YXZ = (YAxis)|(XAxis<<2)|(ZAxis<<4),
		Order_YZX = (YAxis)|(ZAxis<<2)|(XAxis<<4),
		Order_ZXY = (ZAxis)|(XAxis<<2)|(YAxis<<4),
		Order_ZYX = (ZAxis)|(YAxis<<2)|(XAxis<<4)
	};

	SE_INLINE Axes(Axes::Order order)
	{
		Axis0 = (Axes::Type)((order) & 3);
		Axis1 = (Axes::Type)((order>>2) & 3);
		Axis2 = (Axes::Type)((order>>4) & 3);
	}

public:
	Axes::Type Axis0;
	Axes::Type Axis1;
	Axes::Type Axis2;
};

}

#endif 

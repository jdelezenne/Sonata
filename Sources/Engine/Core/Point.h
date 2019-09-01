/*=============================================================================
Point.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_POINT_H_
#define _SE_POINT_H_

#include "Core/Common.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

/** Point.
	Represents an ordered pair of integer x- and y-coordinates that defines a point in a 2D plane.
*/
template <class T>
class Point
{
public:
	static const Point<T> Empty;

public:
	T X;
	T Y;

public:
	/** @name Constructors */
	//@{
	/** Creates an empty point. */
	Point();

	/** Initializes a new instance of the Point class with the specified coordinates.
		@param x The horizontal position of the Point.
		@param y The vertical position of the Point.
	*/
	Point(T x, T y);

	/** Copy constructor. */
	Point(const Point<T>& other);
	//@}

	bool operator==(const Point<T>& other) const;
	bool operator!=(const Point<T>& other) const;

	/** Gets a value indicating whether this Point is empty. */
	bool IsEmpty() const;

	friend Stream& operator<<(Stream& stream, Point<T>& other);
	friend Stream& operator>>(Stream& stream, Point<T>& other);
};

#include "Point.inl"

typedef Point<int32> PointInt;
typedef Point<real> PointReal;

}

#endif 

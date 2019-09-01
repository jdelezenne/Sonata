/*=============================================================================
Rectangle.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RECTANGLE_H_
#define _SE_RECTANGLE_H_

#include "Core/Common.h"
#include "Core/Point.h"
#include "Core/Size.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

/** Rectangle.
	Stores a set of four integers that represent the location and size of a rectangle.
*/
template <class T>
class Rectangle
{
public:
	static const Rectangle<T> Empty;

public:
	T X;
	T Y;
	T Width;
	T Height;

public:
	/** @name Constructors */
	//@{
	/** Creates an empty rectangle. */
	Rectangle();

	/** Initializes a new instance of the Rectangle class with the specified location and size.
		@param x The x-coordinate of the upper-left corner of the Rectangle.
		@param y The y-coordinate of the upper-left corner of the Rectangle.
		@param width The width of the Rectangle.
		@param height The height of the Rectangle.
	*/
	Rectangle(T x, T y, T width, T height);

	/** Initializes a new instance of the Rectangle class with the specified location and size.
		@param location The coordinates of the upper-left corner of the Rectangle.
		@param size The size of the Rectangle.
	*/
	Rectangle(const Point<T>& location, const Size<T>& size);

	/** Copy constructor. */
	Rectangle(const Rectangle<T>& other);
	//@}

	/** Gets the x-coordinate of the upper-left corner of the Rectangle. */
	T GetX() const { return X; }

	/** Sets the x-coordinate of the upper-left corner of the Rectangle. */
	void SetX(T value) { X = value; }

	/** Gets the y-coordinate of the upper-left corner of the Rectangle. */
	T GetY() const { return Y; }

	/** Sets the y-coordinate of the upper-left corner of the Rectangle. */
	void SetY(T value) { Y = value; }

	/** Gets the coordinates of the upper-left corner of the Rectangle. */
	Point<T> GetLocation() const;

	/** Sets the coordinates of the upper-left corner of the Rectangle. */
	void SetLocation(const Point<T>& location);

	/** Gets the width of the Rectangle. */
	T GetWidth() const { return Width; }

	/** Sets the width of the Rectangle. */
	void SetWidth(T value) { Width = value; }

	/** Gets the height of the Rectangle. */
	T GetHeight() const { return Height; }

	/** Sets the height of the Rectangle. */
	void SetHeight(T value) { Height = value; }

	/** Gets the size of the Rectangle. */
	Size<T> GetSize() const;

	/** Sets the size of the Rectangle. */
	void SetSize(const Size<T>& size);

	/** Gets the x-coordinate of the left edge of the Rectangle. */
	T GetLeft() const { return X; }

	/** Gets the x-coordinate of the top edge of the Rectangle. */
	T GetTop() const { return Y; }

	/** Gets the x-coordinate of the left edge of the Rectangle. */
	T GetRight() const { return (X + Width); }

	/** Gets the x-coordinate of the top edge of the Rectangle. */
	T GetBottom() const { return (Y + Height); }

	bool operator==(const Rectangle<T>& other) const;
	bool operator!=(const Rectangle<T>& other) const;

	/** Gets a value indicating whether the Rectangle is empty. */
	bool IsEmpty() const;

	/** Determines if the specified point is contained within the rectangular region defined by the Rectangle. */
	bool Contains(T x, T y) const;
	bool Contains(const Point<T>& value) const { return Contains(value.X, value.Y); }

	/** Adjusts the location of the Rectangle by the specified amount. */
	void Offset(T x, T y);
	void Offset(const Point<T>& value) { return Offset(value.X, value.Y); }

	/** Inflates the Rectangle by the specified amount. */
	void Inflate(T width, T height);
	void Inflate(const Size<T>& value) { return Inflate(value.width, value.height); }

	/** Determines the Rectangle structure that represents the intersection of two rectangles. */
	void Intersect(const Rectangle<T>& value);
	static Rectangle<T> Intersect(const Rectangle<T>& left, const Rectangle<T>& right);

	/** Determines the Rectangle structure that represents the union of two rectangles. */
	void Union(const Rectangle<T>& value);
	static Rectangle<T> Union(const Rectangle<T>& left, const Rectangle<T>& right);

	friend Stream& operator<<(Stream& stream, Rectangle<T>& other);
	friend Stream& operator>>(Stream& stream, Rectangle<T>& other);
};

#include "Rectangle.inl"

typedef Rectangle<int32> RectangleInt;
typedef Rectangle<real> RectangleReal;

}

#endif 

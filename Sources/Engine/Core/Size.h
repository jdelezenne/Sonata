/*=============================================================================
Size.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SIZE_H_
#define _SE_SIZE_H_

#include "Core/Common.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

/** Size.
	Represents an ordered pair of integer that defines a 2D size.
*/
template <class T>
class Size
{
public:
	static const Size<T> Empty;

public:
	T Width;
	T Height;

public:
	/** @name Constructors */
	//@{
	/** Creates an empty point. */
	Size();

	/** Initializes a new instance of the Size class from the specified dimensions.
		@param width The width component of the new Size.
		@param height The height component of the new Size.
	*/
	Size(T width, T height);

	/** Copy constructor. */
	Size(const Size<T>& other);
	//@}

	bool operator==(const Size<T>& other) const;
	bool operator!=(const Size<T>& other) const;

	/** Gets a value indicating whether this Size is empty. */
	bool IsEmpty() const;

	friend Stream& operator<<(Stream& stream, Size<T>& other);
	friend Stream& operator>>(Stream& stream, Size<T>& other);
};

#include "Size.inl"

typedef Size<int32> SizeInt;
typedef Size<real> SizeReal;

}

#endif 

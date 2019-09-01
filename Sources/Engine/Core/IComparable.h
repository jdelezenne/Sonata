/*=============================================================================
IComparable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ICOMPARABLE_H_
#define _SE_ICOMPARABLE_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Interface for the comparable values.
	Defines a generalized comparison method that a type implements to create a type-specific comparison method.
*/
template <class T>
class IComparable
{
public:
	enum Result
	{
		/// This instance is less than value.
		Less,
		
		/// This instance equals value.
		Equal,
		
		/// This instance is greater than value.
		Greater
	};

	/// Compares the current instance with another object of the same type.
	virtual int CompareTo(const T value) const = 0;
};

}

#endif 

/*=============================================================================
IComparer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ICOMPARER_H_
#define _SE_ICOMPARER_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Interface for the comparers.
	Exposes a method that compares two objects.
*/
template <class T>
class IComparer
{
public:
	enum Result
	{
		Less, /// x is less than y.
		Equal, /// x equals y.
		Greater /// x is greater than y.
	};

	/// Compares two objects and returns a value indicating whether one is less than, equal to or greater than the value.
	virtual int Compare(const T& x, const T& y) const = 0;
};

}

#endif 

/*=============================================================================
Hashtable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_HASHTABLE_H_
#define _SE_HASHTABLE_H_

#include <hash_map>

#include "Core/Common.h"
#include "Core/Containers/IDictionaryIterator.h"

namespace SonataEngine
{

/** Represents a collection of key/value pairs that are organized based on the hash code of the key. */
template <class TKey, class TValue>
class Hashtable
{
};

#include "Hashtable.inl"

}

#endif

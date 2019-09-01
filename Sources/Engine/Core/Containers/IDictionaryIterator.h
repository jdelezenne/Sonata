/*=============================================================================
IDictionaryIterator.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IDICTIONARYITERATOR_H_
#define _SE_IDICTIONARYITERATOR_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Defines a key/value pair that can be set or retrieved. */
template <class TKey, class TValue>
class IKeyValuePair
{
public:
	/// Gets the key in the key-and-value pair.
	virtual TKey Key() const { return TKey(); }

	/// Gets the value in the key-and-value pair.
	virtual TValue Value() const { return TValue(); }
};


/** Defines a dictionary key/value pair that can be set or retrieved. */
template <class TKey, class TValue>
class IDictionaryEntry
{
public:
	/// Gets the key in the key-and-value pair.
	virtual TKey GetKey() const { return TKey(); }

	/// Sets the key in the key-and-value pair.
	virtual void SetKey(const TKey& key) { }

	/// Gets the value in the key-and-value pair.
	virtual TValue GetValue() const { return TValue(); }

	/// Sets the value in the key-and-value pair.
	virtual void SetValue(const TValue& value) { }
};


/** Interface for the dictionary iterators. */
template <class TKey, class TValue>
class IDictionaryIterator : public IIterator< IKeyValuePair<TKey, TValue> >
{
public:
	/// Gets both the key and the value of the current dictionary entry. 
	virtual IDictionaryEntry<TKey, TValue> Entry() const = 0;

	/// Gets the key of the current dictionary entry. 
	virtual TKey Key() const = 0;

	/// Gets the value of the current dictionary entry. 
	virtual TValue Value() const = 0;
};

}

#endif 

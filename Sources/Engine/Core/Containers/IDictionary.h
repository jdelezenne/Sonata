/*=============================================================================
IDictionary.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IDICTIONARY_H_
#define _SE_IDICTIONARY_H_

#include "Core/Common.h"
#include "Core/Containers/IDictionaryIterator.h"
#include "Core/Containers/List.h"

namespace SonataEngine
{

/** Interface for the dicitonaries. */
template <class TKey, class TValue>
class IDictionary : public IContainer< IKeyValuePair<TKey, TValue> >
{
	/// Operators
	virtual TValue& operator[](const TKey& key) = 0;
	virtual const TValue& operator[](const TKey& key) const = 0;

	/// Removes all elements from the Dictionary.
	virtual void Clear() = 0;

	/// Gets the number of key-and-value pairs contained in the Dictionary.
	virtual int Count() const = 0;

	/// Returns whether the Dictionary is empty.
	virtual bool IsEmpty() const = 0;

	/// Adds an element with the specified key and value into the Dictionary.
	virtual void Add(const TKey& key, const TValue& value) = 0;

	/// Removes the element with the specified key from the Dictionary.
	virtual void Remove(const TKey& key) = 0;

	/// Determines whether the Dictionary contains a specific key.
	virtual bool Contains(const TKey& key) const = 0;

	/// Determines whether the Dictionary contains a specific key.
	virtual bool ContainsKey(const TKey& key) const = 0;

	/// Determines whether the Dictionary contains a specific value.
	virtual bool ContainsValue(const TValue& value) const = 0;

	/// Gets the value associated with the specified key.
	virtual TValue& GetItem(const TKey& key) = 0;
	virtual const TValue& GetItem(const TKey& key) const = 0;

	/// Sets the value associated with the specified key.
	virtual void SetItem(const TKey& key, const TValue& value) = 0;

	/// Gets a container containing the keys in the Dictionary.
	virtual BaseArray<TKey> Keys() const = 0;

	/// Gets a container containing the valuesin the Dictionary.
	virtual BaseArray<TValue> Values() const = 0;
};

}

#endif

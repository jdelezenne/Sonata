/*=============================================================================
Dictionary.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DICTIONARY_H_
#define _SE_DICTIONARY_H_

#include <map>

#include "Core/Common.h"
#include "Core/Containers/IDictionary.h"

namespace SonataEngine
{

/** Represents a collection of keys and values. */
template <class TKey, class TValue>
class Dictionary : public IDictionary<TKey, TValue>
{
protected:
	typedef typename std::map<TKey, TValue> InnerType;
	typedef typename std::pair<TKey, TValue> InnerPair;
	typename InnerType _internal;

public:
	class DictionaryIterator;

	class KeyValuePair : public IKeyValuePair<TKey, TValue>
	{
	protected:
		InnerPair _pair;

	public:
		KeyValuePair(const TKey& key, const TValue& value);

		virtual TKey Key() const;

		virtual TValue Value() const;
	};

	class DictionaryEntry : public IDictionaryEntry<TKey, TValue>
	{
	protected:
		typename InnerType::const_iterator _iterator;

	public:
		DictionaryEntry();
		DictionaryEntry(typename InnerType::const_iterator iterator);

		virtual TKey GetKey() const;

		virtual void SetKey(const TKey& key);

		virtual TValue GetValue() const;

		virtual void SetValue(const TValue& value);
	};

	class DictionaryIterator : public IDictionaryIterator<TKey, TValue>
	{
	protected:
		const Dictionary<TKey, TValue>* _dictionary;
		typename InnerType::const_iterator _it;
		bool _isFirst;

	public:
		DictionaryIterator(const Dictionary<TKey, TValue>* dictionary);
		virtual DictionaryIterator& operator++();

		virtual IKeyValuePair<TKey, TValue> Current() const;

		virtual bool Next();

		virtual typename IDictionaryEntry<TKey, TValue> Entry() const;

		virtual TKey Key() const;

		virtual TValue Value() const;
	};

	friend DictionaryIterator;

public:
	typedef typename DictionaryIterator Iterator;

	Dictionary();
	Dictionary(int size);
	Dictionary(const Dictionary<TKey, TValue>& value);
	virtual ~Dictionary();

	virtual Dictionary<TKey, TValue>& operator=(const Dictionary<TKey, TValue>& value);
	virtual TValue& operator[](const TKey& key);
	virtual const TValue& operator[](const TKey& key) const;

	virtual Iterator GetIterator() const;

	virtual void Clear();

	virtual int Count() const;

	virtual bool IsEmpty() const;

	virtual void Add(const TKey& key, const TValue& value);

	virtual void Remove(const TKey& key);

	virtual bool Contains(const TKey& key) const;

	virtual bool ContainsKey(const TKey& key) const;

	virtual bool ContainsValue(const TValue& value) const;

	virtual TValue& GetItem(const TKey& key);
	virtual const TValue& GetItem(const TKey& key) const;

	virtual void SetItem(const TKey& key, const TValue& value);

	virtual BaseArray<TKey> Keys() const;

	virtual BaseArray<TValue> Values() const;
};

#include "Dictionary.inl"

}

#endif
